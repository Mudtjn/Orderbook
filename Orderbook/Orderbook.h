#pragma once
#include "Usings.h"
#include "Order.h"
#include "Trades.h"
#include "OrderbookLevelInfos.h"
#include "OrderModify.h"
#include <map>
#include <unordered_map>

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

	bool canMatch(Side side, Price price) const; 

	Trades MatchOrders();

public:
	Trades AddOrder(OrderPointer order);
	void CancelOrder(OrderId orderId);
	Trades ModifyOrder(OrderModify order); 
	std::size_t Size() const; 

	OrderbookLevelInfos GetOrderInfos() const;

};