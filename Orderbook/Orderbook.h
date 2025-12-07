#pragma once
#include "Usings.h"
#include "Order.h"
#include "Trades.h"
#include "OrderbookLevelInfos.h"
#include "OrderModify.h"
#include <map>
#include <unordered_map>
#include <thread>
#include <mutex>

class Orderbook
{
private:
	struct OrderEntry
	{
		OrderPointer order_{ nullptr };
		OrderPointers::iterator location_;
	};

	// stores each price level metadata
	struct LevelData {
		Quantity quantity_{};
		Quantity count_{}; 

		enum class Action {
			Add, 
			Remove, 
			Match
		};
	};

	// stores each price level metadata
	std::unordered_map <Price, LevelData> data_; 
	std::map <Price, OrderPointers, std::greater<Price>> bids_;
	std::map <Price, OrderPointers, std::less<Price>> asks_;
	std::unordered_map<OrderId, OrderEntry> orders_;
	
	// locking order table and thread to delete for good for day orders
	mutable std::mutex ordersMutex_;
	std::thread ordersPruneThread_;
	// synchronization used with mutex
	// to block until specific condition is met
	std::condition_variable shutdownConditionVariable_;
	std::atomic<bool> shutdown_{ false };
	bool canMatch(Side side, Price price) const; 

	Trades MatchOrders();
	void PruneGoodForDayOrders();
	void CancelOrders(OrderIds orderIds); 
	void CancelOrderInternal(OrderId orderId);

	void OnOrderCancelled(OrderPointer order);
	void OnOrderAdded(OrderPointer order); 
	void OnOrderMatched(Price price, Quantity quantity, bool isFullyFilled); 
	void UpdateLevelData(Price price, Quantity quantity, LevelData::Action action);

	bool CanFullyFill(Side side, Price price, Quantity quantity) const; 
public:
	Orderbook();
	Orderbook(const Orderbook&) = delete; 
	void operator=(const Orderbook&) = delete; 
	Orderbook(Orderbook&) = delete; 
	void operator=(Orderbook&) = delete; 
	Trades AddOrder(OrderPointer order);
	void CancelOrder(OrderId orderId);
	Trades ModifyOrder(OrderModify order); 
	std::size_t Size() const; 
	OrderbookLevelInfos GetOrderInfos() const;
	~Orderbook(); 
};