import grpc
from concurrent import futures
from protos import Orders_pb2, Orders_pb2_grpc
import orderbook as OrderbookModule
from util.helper_functions import getTrades, getSideEnum

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
        orderbook_response = self.my_orderbook.addOrder(new_order)
        response = Orders_pb2.AddOrderResponse(
            trades = [getTrades(x) for x in orderbook_response if orderbook_response]
        )
        return response

    def AddMarketOrder(self, request, context):
        # Market Order
        print("Add Market Order Invoked")
        new_order = OrderbookModule.Order( 
            getSideEnum(request.side), 
            request.quantity
        )
        orderbook_response = self.my_orderbook.addOrder(new_order)
        response = Orders_pb2.AddOrderResponse(
            trades = [getTrades(x) for x in orderbook_response if orderbook_response]
        )
        return response

    def CancelOrder(self, request, context):
        # Cancel Order
        print("Cancel Order Invoked")
        self.my_orderbook.cancelOrder(request.order_id)
        return grpc.experimental.empty_pb2.Empty()

def serve():
    main_orderbook = OrderbookModule.Orderbook()
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=20))
    # ord1 = main_orderbook.addOrder(
    #     OrderbookModule.Order(
    #         OrderbookModule.OrderType.GoodTillCancel,
    #         OrderbookModule.Side.Buy,
    #         10,
    #         50
    #     )
    # )
    # print("----------------------------------------")
    # print(ord1)
    # ord2 = main_orderbook.addOrder(
    #     OrderbookModule.Order(
    #         OrderbookModule.OrderType.GoodTillCancel, 
    #         OrderbookModule.Side.Buy, 
    #         10, 
    #         51 
    #     )
    # )
    Orders_pb2_grpc.add_OrderbookServiceServicer_to_server(
        OrderbookServicer(main_orderbook), server
    )
    server.add_insecure_port('[::]:50051')  # IPv6 wildcard
    server.start()
    print("Server running on port 50051")
    server.wait_for_termination()

if __name__ == "__main__": 
    serve()