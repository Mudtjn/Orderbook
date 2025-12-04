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
public:
	Trades AddOrder(OrderPointer order);
	void CancelOrder(OrderId orderId);
	Trades ModifyOrder(OrderModify order); 
	std::size_t Size() const; 
	OrderbookLevelInfos GetOrderInfos() const;
	
};