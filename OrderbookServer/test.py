import orderbook

myOrderbook = orderbook.Orderbook()

order = orderbook.Order(orderbook.Side.Buy, 10)
print(order)
order1 = orderbook.Order(orderbook.Side.Buy, 10)
print(order1)
print(myOrderbook.addOrder(order))

info = myOrderbook.getOrderInfos()
print(info.Bids)
print(info.Asks)
