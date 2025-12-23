#include "include/BindOrder.h"
#include "include/OrderTypes.h"
#include <string>

void bindOrder(py::module &m) {
  py::class_<Order, std::shared_ptr<Order>>(m, "Order", "constructor for Order")
      .def(py::init<OrderType, OrderId, Side, Price, Quantity>(),
           py::arg("OrderType"), py::arg("OrderId"), py::arg("side"),
           py::arg("price"), py::arg("quantity"),
           "constructor for modify orders")
      .def(py::init<OrderType, Side, Price, Quantity>(), py::arg("OrderType"),
           py::arg("side"), py::arg("price"), py::arg("quantity"),
           "constructor for order with required params")
      .def(py::init<Side, Quantity>(), py::arg("side"), py::arg("quantity"),
           "constructor for market orders")
      .def_property_readonly("orderType", &Order::GetOrderType,
                             py::return_value_policy::reference_internal,
                             "Get order type")
      .def_property_readonly("orderId", &Order::GetOrderId,
                             py::return_value_policy::reference_internal,
                             "Get order id")
      .def_property_readonly("side", &Order::GetSide,
                             py::return_value_policy::reference_internal,
                             "Get Order Side")
      .def_property_readonly("price", &Order::GetPrice,
                             py::return_value_policy::reference_internal,
                             "Get Order price")
      .def_property_readonly("initialQuantity", &Order::GetInitialQuantity,
                             py::return_value_policy::reference_internal,
                             "Get Order initialQuantity")
      .def_property_readonly("remainingQuantity", &Order::GetRemainingQuantity,
                             py::return_value_policy::reference_internal,
                             "Get Order remainingQuantity")
      .def_property_readonly("filledQuantity", &Order::GetFilledQuantity,
                             py::return_value_policy::reference_internal,
                             "Get Order filledQuantity")

      .def("isFilled", &Order::isFilled)
      .def("__eq__",
           [](const Order &a, const Order &b) {
             return a.GetOrderId() == b.GetOrderId();
           })
      .def("__repr__",
           [](const Order &order) {
             return "Order(OrderId=" + std::to_string(order.GetOrderId()) +
                    " ,OrderType=" + orderTypeToString(order.GetOrderType()) +
                    +" ,Side=" + sideToString(order.GetSide()) +
                    " ,Price=" + std::to_string(order.GetPrice()) +
                    " ,initialQuantity=" +
                    std::to_string(order.GetInitialQuantity()) +
                    " ,FilledQuantity=" +
                    std::to_string(order.GetFilledQuantity());
           }

      );
}
