from protos import Orders_pb2, Orders_pb2_grpc
import orderbook as OrderbookModule

def getTrades(trades: OrderbookModule.Trade) -> Orders_pb2.Trade:
    proto_trade = Orders_pb2.Trade()

    bid_trade = proto_trade.bid_trade
    bid_trade.order_id = trades.bidTrade.orderId
    bid_trade.price = trades.bidTrade.price
    bid_trade.quantity = trades.bidTrade.quantity

    ask_trade = proto_trade.ask_trade
    ask_trade.order_id = trades.askTrade.orderId
    ask_trade.price = trades.askTrade.price
    ask_trade.quantity = trades.askTrade.quantity
    print(proto_trade)
    return proto_trade

def getSideEnum(side_value: int) -> OrderbookModule.Side:
    if side_value == 0:
        return OrderbookModule.Side.Buy
    elif side_value == 1:
        return OrderbookModule.Side.Sell
    else:
        raise ValueError("Invalid side value")
