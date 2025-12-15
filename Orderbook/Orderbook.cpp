#include "include/Orderbook.h"
#include "include/BindOrderbook.h"
#include <chrono>
#include <ctime>
#include <numeric>
#include <optional>

void Orderbook::OnOrderAdded(OrderPointer order) {
  UpdateLevelData(order->GetPrice(), order->GetRemainingQuantity(),
                  LevelData::Action::Add);
}

void Orderbook::OnOrderCancelled(OrderPointer order) {
  UpdateLevelData(order->GetPrice(), order->GetRemainingQuantity(),
                  LevelData::Action::Remove);
}

void Orderbook::OnOrderMatched(Price price, Quantity quantity,
                               bool isFullyFilled) {
  UpdateLevelData(price, quantity,
                  isFullyFilled ? LevelData::Action::Remove
                                : LevelData::Action::Match);
}

void Orderbook::UpdateLevelData(Price price, Quantity quantity,
                                LevelData::Action action) {
  auto &data = data_[price];

  data.count_ += action == LevelData::Action::Remove ? -1
                 : action == LevelData::Action::Add  ? 1
                                                     : 0;

  if (action == LevelData::Action::Remove ||
      action == LevelData::Action::Match) {
    data.quantity_ -= quantity;
  } else {
    data.quantity_ += quantity;
  }

  if (data.count_ == 0) {
    data_.erase(price);
  }
}

bool Orderbook::canMatch(Side side, Price price) const {
  if (side == Side::Buy) {
    if (asks_.empty()) {
      return false;
    }

    const auto &[bestAsk, _] = *asks_.begin();
    return price >= bestAsk;
  } else {
    if (bids_.empty()) {
      return false;
    }
    const auto &[bestBid, _] = *bids_.begin();
    return price <= bestBid;
  }
}

void Orderbook::CancelOrderInternal(OrderId orderId) {
  if (!orders_.contains(orderId))
    return;

  const auto [order, iterator] = orders_.at(orderId);
  orders_.erase(orderId);

  // remove from asks map
  if (order->GetSide() == Side::Sell) {
    auto price = order->GetPrice();
    auto &orders = asks_.at(price);
    orders.erase(iterator);
    if (orders.empty()) {
      asks_.erase(price);
    }
  } else {
    auto price = order->GetPrice();
    auto &orders = bids_.at(price);
    orders.erase(iterator);
    if (orders.empty()) {
      bids_.erase(price);
    }
  }

  OnOrderCancelled(order);
}

void Orderbook::CancelOrders(OrderIds orderIds) {
  std::scoped_lock orderLock{ordersMutex_};
  for (const auto &orderId : orderIds) {
    CancelOrderInternal(orderId);
  }
}

void Orderbook::PruneGoodForDayOrders() {
  using namespace std::chrono;

  const auto end = hours(16);

  while (1) {
    // returns time_point representing now
    const auto now = system_clock::now();
    // convert to time_t to represent seconds since unix epoch
    const auto now_c = system_clock::to_time_t(now);
    std::tm now_parts;
    localtime_r(&now_c, &now_parts);

    if (now_parts.tm_hour >= end.count()) {
      // moves the day to tomorrow
      now_parts.tm_mday += 1;
    }

    now_parts.tm_hour = end.count();
    now_parts.tm_min = 0;
    now_parts.tm_sec = 0;

    auto next = system_clock::from_time_t(mktime(&now_parts));
    auto till = next - now + milliseconds(100);

    {
      // can only lock one mutex at a time to avoid deadlock
      std::unique_lock ordersLock{ordersMutex_};
      // shutdown_.load(std::memory_order_acquire)
      // no load or store after it can move before it
      if (shutdown_.load(std::memory_order_acquire) ||
          // object to be locked by current thread
          // max time to wait to acquire the lock
          shutdownConditionVariable_.wait_for(ordersLock, till) ==
              std::cv_status::no_timeout)
        return;
    }

    OrderIds orderIds;

    {
      std::scoped_lock ordersLock{ordersMutex_};
      for (const auto &[orderId, entry] : orders_) {
        const auto &[order, _] = entry;
        if (order->GetOrderType() == OrderType::GoodForDay)
          continue;
        orderIds.push_back(order->GetOrderId());
      }
    }
    CancelOrders(orderIds);
  }
}

Trades Orderbook::MatchOrders() {
  Trades trades;
  trades.reserve(orders_.size());

  while (true) {
    if (bids_.empty() || asks_.empty())
      break;

    auto &[bidPrice, bids] = *bids_.begin();
    auto &[askPrice, asks] = *asks_.begin();

    // highest bid < smallest ask
    if (bidPrice < askPrice) {
      break;
    }

    while (!bids.empty() && !asks.empty()) {
      auto bid = bids.front();
      auto ask = asks.front();

      Quantity quantity =
          std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());
      bid->Fill(quantity);
      ask->Fill(quantity);

      if (bid->isFilled()) {
        bids.pop_front();
        orders_.erase(bid->GetOrderId());
      }

      if (ask->isFilled()) {
        asks.pop_front();
        orders_.erase(ask->GetOrderId());
      }

      trades.push_back(
          Trade{TradeInfo{bid->GetOrderId(), bid->GetPrice(), quantity},
                TradeInfo{ask->GetOrderId(), ask->GetPrice(), quantity}});

      OnOrderMatched(bid->GetPrice(), quantity, bid->isFilled());
      OnOrderMatched(ask->GetPrice(), quantity, ask->isFilled());
    }

    if (bids.empty()) {
      bids_.erase(bidPrice);
      data_.erase(bidPrice);
    }
    if (asks.empty()) {
      asks_.erase(askPrice);
      data_.erase(askPrice);
    }
  }

  if (!bids_.empty()) {
    auto &[_, bids] = *bids_.begin();
    auto &order = bids.front();
    if (order->GetOrderType() == OrderType::FillAndKill) {
      CancelOrder(order->GetOrderId());
    }
  }

  if (!asks_.empty()) {
    auto &[_, asks] = *asks_.begin();
    auto &order = asks.front();
    if (order->GetOrderType() == OrderType::FillAndKill) {
      CancelOrder(order->GetOrderId());
    }
  }

  return trades;
}

// checks if an order can be fully filled
bool Orderbook::CanFullyFill(Side side, Price price, Quantity quantity) const {
  if (!canMatch(side, price))
    return false;

  std::optional<Price> threshold;
  if (side == Side::Buy) {
    const auto [askPrice, _] = *asks_.begin();
    threshold = askPrice;
  } else {
    const auto [bidPrice, _] = *bids_.begin();
    threshold = bidPrice;
  }

  for (const auto &[levelPrice, levelData] : data_) {
    if (threshold.has_value() &&
            (side == Side::Buy && threshold.value() > levelPrice) ||
        (side == Side::Sell && threshold.value() < levelPrice))
      continue;

    if ((side == Side::Buy && levelPrice > price) ||
        (side == Side::Sell && levelPrice < price))
      continue;

    if (quantity <= levelData.quantity_) {
      return true;
    }
    quantity -= levelData.quantity_;
  }
  return false;
}

Trades Orderbook::AddOrder(OrderPointer order) {
  // the mutex serializes running the function;
  // only one thread can run any function
  // scoped lock unlocks when going out of scope
  std::scoped_lock orderLock{ordersMutex_};
  if (orders_.contains(order->GetOrderId()))
    return {};

  if (order->GetOrderType() == OrderType::Market) {
    if (order->GetSide() == Side::Buy && !asks_.empty()) {
      const auto &[worstAsk, _] = *asks_.rbegin();
      order->ToGoodTillCancel(worstAsk);
    } else if (order->GetSide() == Side::Sell && !bids_.empty()) {
      const auto &[worstBid, _] = *bids_.rbegin();
      order->ToGoodTillCancel(worstBid);
    } else
      return {};
  }

  if (order->GetOrderType() == OrderType::FillAndKill &&
      !canMatch(order->GetSide(), order->GetPrice()))
    return {};

  if (order->GetOrderType() == OrderType::FillOrKill &&
      !CanFullyFill(order->GetSide(), order->GetPrice(),
                    order->GetInitialQuantity()))
    return {};

  OrderPointers::iterator iterator;

  if (order->GetSide() == Side::Buy) {
    auto &orders = bids_[order->GetPrice()];
    orders.push_back(order);
    iterator = std::next(orders.begin(), orders.size() - 1);
  } else {
    auto &orders = asks_[order->GetPrice()];
    orders.push_back(order);
    iterator = std::next(orders.begin(), orders.size() - 1);
  }
  orders_.insert({order->GetOrderId(), OrderEntry{order, iterator}});
  OnOrderAdded(order);
  return MatchOrders();
}

void Orderbook::CancelOrder(OrderId orderId) {
  std::scoped_lock orderLock{ordersMutex_};
  CancelOrderInternal(orderId);
}

Trades Orderbook::ModifyOrder(OrderModify order) {
  OrderType orderType;
  {
    std::scoped_lock orderLock{ordersMutex_};
    if (!orders_.contains(order.GetOrderId()))
      return {};
    const auto &[existingOrder, _] = orders_.at(order.GetOrderId());
    orderType = existingOrder->GetOrderType();
  }

  CancelOrder(order.GetOrderId());
  return AddOrder(order.ToOrderPointer(orderType));
}

std::size_t Orderbook::Size() const {
  std::scoped_lock orderLock{ordersMutex_};
  return orders_.size();
}

OrderbookLevelInfos Orderbook::GetOrderInfos() const {
  LevelInfos bidInfos, askInfos;
  bidInfos.reserve(orders_.size());
  askInfos.reserve(orders_.size());

  auto CreateLevelInfos = [](Price price, OrderPointers &orders) {
    return LevelInfo{
        price, std::accumulate(orders.begin(), orders.end(), (Quantity)0,
                               [](Quantity sum, const OrderPointer &order) {
                                 return sum + order->GetRemainingQuantity();
                               })};
  };
  for (const auto &[price, orders] : bids_) {
    bidInfos.push_back(
        CreateLevelInfos(price, const_cast<OrderPointers &>(orders)));
  }
  for (const auto &[price, orders] : asks_) {
    askInfos.push_back(
        CreateLevelInfos(price, const_cast<OrderPointers &>(orders)));
  }
  return OrderbookLevelInfos{bidInfos, askInfos};
}

Orderbook::Orderbook()
    : ordersPruneThread_{[this] { PruneGoodForDayOrders(); }} {}

Orderbook::~Orderbook() {
  // shutdown stores true with release semantics
  // all operations before the store in this thread
  shutdown_.store(true, std::memory_order_release);
  // notify the condition variable to wake up the thread
  shutdownConditionVariable_.notify_one();
  // join thread back to main thread
  ordersPruneThread_.join();
}

PYBIND11_MODULE(orderbook, m, py::mod_gil_not_used()) {
  m.doc() = "pybind orderbook plugin";
  bindSide(m);
  bindOrderTypes(m);
  bindLevelInfo(m);
  bindOrder(m);
  bindOrderModify(m);
  bindTrades(m);
  bindOrderbookLevelInfos(m);
  bindOrderbook(m);
}
