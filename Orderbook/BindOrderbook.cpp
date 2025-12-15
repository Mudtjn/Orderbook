#include "include/BindOrderbook.h"
#include "include/Orderbook.h"

void bindOrderbook(py::module &m) {
  py::class_<Orderbook>(m, "Orderbook", "constructor for Orderbook")
      .def(py::init<>(), "constructor for orderbook")
      .def("addOrder", &Orderbook::AddOrder, "function to Add Order")
      .def("modifyOrder", &Orderbook::ModifyOrder, "function to modify order")
      .def("getOrderInfos", &Orderbook::GetOrderInfos,
           "function to get Current orders in orderbook")
      .def("size", &Orderbook::Size, "function to get size of the orderbook")
      .def("cancelOrder", &Orderbook::CancelOrder, "function to cancel order");
}
