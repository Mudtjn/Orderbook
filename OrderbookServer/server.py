import grpc
from concurrent import futures
from protos import Orders_pb2, Orders_pb2_grpc
import orderbook as OrderbookModule

def getSideEnum(side_value: int) -> OrderbookModule.Side:
    if side_value == 0:
        return OrderbookModule.Side.Buy
    elif side_value == 1:
        return OrderbookModule.Side.Sell
    else:
        raise ValueError("Invalid side value")

class OrderbookServicer(Orders_pb2_grpc.OrderbookService): 
    def __init__(self, orderbook_instance: OrderbookModule.Orderbook):
        self.my_orderbook = orderbook_instance

    def AddOrder(self, request, context): 
        # Limit Order
        print("Add Order Invoked")
        new_order = OrderbookModule.Order(
            request.order_type, 
            request.side, 
            request.price, 
            request.quantity
        )
        added_order = self.my_orderbook.addOrder(new_order)
        response = Orders_pb2.AddOrderResponse(
            order_id=added_order.id,
            order_type=added_order.order_type,
            side=added_order.side,
            price=added_order.price,
            initial_quantity=added_order.initial_quantity,
            filled_quantity=added_order.filled_quantity
        )
        print(response)
        return response

    def AddMarketOrder(self, request, context):
        # Market Order
        print("Add Market Order Invoked")
        new_order = OrderbookModule.Order( 
            getSideEnum(request.side), 
            request.quantity
        )
        added_order = self.my_orderbook.addOrder(new_order)
        print(added_order)
        response = Orders_pb2.AddOrderResponse(
            order_id=added_order.OrderId,
            order_type=added_order.OrderType,
            side=added_order.Side,
            price=added_order.Price,
            initial_quantity=added_order.initialQuantity,
            filled_quantity=added_order.FilledQuantity
        )

        return response

def serve():
    main_orderbook = OrderbookModule.Orderbook()
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=20))
    ord1 = main_orderbook.addOrder(
        OrderbookModule.Order(
            OrderbookModule.OrderType.GoodTillCancel,
            OrderbookModule.Side.Buy,
            10,
            200
        )
    )
    print("----------------------------------------")
    print(ord1)
    Orders_pb2_grpc.add_OrderbookServiceServicer_to_server(
        OrderbookServicer(main_orderbook), server
    )
    server.add_insecure_port('[::]:50051')  # IPv6 wildcard
    server.start()
    print("Server running on port 50051")
    server.wait_for_termination()

if __name__ == "__main__": 
    serve()