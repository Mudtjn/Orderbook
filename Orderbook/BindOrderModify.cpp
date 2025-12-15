#include "include/BindOrderModify.h"
#include "include/Side.h"

void bindOrderModify(py::module &m) {
  py::class_<OrderModify, std::shared_ptr<OrderModify>>(m, "OrderModify",
                                                        "Ordermodify struct")
      .def(py::init<const OrderId, const Side, const Price, const Quantity>(),
           py::arg("orderId"), py::arg("side"), py::arg("price"),
           py::arg("quantity"), "Construct an modify order with information")
      .def("GetOrderId", &OrderModify::GetOrderId,
           py::return_value_policy::reference_internal,
           "Get orderId to be modified")
      .def("GetSide", &OrderModify::GetSide,
           py::return_value_policy::reference_internal,
           "Get side of the modified order")
      .def("GetPrice", &OrderModify::GetPrice,
           py::return_value_policy::reference_internal,
           "Get price of the modified order")
      .def("GetQuantity", &OrderModify::GetQuantity,
           py::return_value_policy::reference_internal,
           "Get quantity of the modified order")

      .def_property_readonly("OrderId", &OrderModify::GetOrderId,
                             py::return_value_policy::reference_internal,
                             "modify order OrderId")
      .def_property_readonly("Side", &OrderModify::GetSide,
                             py::return_value_policy::reference_internal,
                             "modify order Side")
      .def_property_readonly("Price", &OrderModify::GetPrice,
                             py::return_value_policy::reference_internal,
                             "modify order price")
      .def_property_readonly("Quantity", &OrderModify::GetQuantity,
                             py::return_value_policy::reference_internal,
                             "modify order quantity")

      // TODO: implement binding for ToOrderPointer function

      .def("__repr__", [](const OrderModify &orderModify) {
        return "OrderModify(OrderId=" +
               std::to_string(orderModify.GetOrderId()) +
               " ,Side=" + sideToString(orderModify.GetSide()) +
               " ,Price=" + std::to_string(orderModify.GetPrice()) +
               " ,Quantity=" + std::to_string(orderModify.GetQuantity()) + ")";
      });
}
