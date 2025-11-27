#pragma once
#include "Side.h"
#include "OrderTypes.h"
#include "Usings.h"
#include "Constants.h"
#include <list>
#include <format>

class Order {
public:
	Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
		:orderType_{ orderType },
		side_{ side },
		orderId_{ orderId },
		price_{ price },
		initialQuantity_{ quantity },
		remainingQuantity_{ quantity }
	{
	}

	Order(OrderId orderId, Side side, Quantity quantity)
		: orderType_ {OrderType::Market}, 
		  side_{ side },
		  orderId_{ orderId },
		  price_{ Constants::InvalidPrice },
		  initialQuantity_{ quantity },
		remainingQuantity_{ quantity }
	{}

	OrderId GetOrderId() const { return orderId_; }
	Side GetSide() const { return side_; }
	Price GetPrice() const { return price_; }
	OrderType GetOrderType() const { return orderType_; }
	Quantity GetInitialQuantity() const { return initialQuantity_; }
	Quantity GetRemainingQuantity() const { return remainingQuantity_; }
	Quantity GetFilledQuantity() const { return GetInitialQuantity() - GetRemainingQuantity(); }

	bool isFilled() const { return GetRemainingQuantity() == 0; }

	void Fill(Quantity quantity) {
		if (quantity > GetRemainingQuantity()) {
			throw std::logic_error(std::format("Order ({}) cannot be filled for more than its remaining quantity", GetOrderId()));
		}

		remainingQuantity_ = remainingQuantity_ - quantity;
	}

	void ToGoodTillCancel(Price price) {
		if(GetOrderType() != OrderType::Market) {
			throw std::logic_error(std::format("Order ({}) cannot have its price adjusted, only market orders can.", GetOrderId()));
		}
		price_ = price; 
		orderType_ = OrderType::GoodTillCancel;
	}

private:
	OrderType orderType_;
	Side side_;
	OrderId orderId_;
	Price price_;
	Quantity initialQuantity_;
	Quantity remainingQuantity_;
};

// points to an order in multiple places
using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;
// add order
// cancel order
// modify order -> cancel + add 