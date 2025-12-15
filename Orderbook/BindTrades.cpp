#include "include/BindTrades.h"
#include "include/Trades.h"
#include "pybind11/detail/common.h"
#include <string>

void bindTrades(py::module &m) {
  py::class_<TradeInfo>(m, "TradeInfo", "Constructor for TradeInfo")
      .def_readonly("orderId", &TradeInfo::orderId_)
      .def_readonly("price", &TradeInfo::price_)
      .def_readonly("quantity", &TradeInfo::quantity_)
      .def("__repr__", [](const TradeInfo &trade) {
        return "Trade(OrderID=" + std::to_string(trade.orderId_) +
               " ,Price=" + std::to_string(trade.price_) +
               " ,Quantity=" + std::to_string(trade.quantity_);
      });

  py::class_<Trade>(m, "Trade", "Constructor for Trade")
      .def(py::init<const TradeInfo &, const TradeInfo &>(),
           py::arg("bidTrade"), py::arg("askTrade"), "Constructor for Trade")
      .def_property_readonly("bidTrade", &Trade::GetBidTrade,
                             py::return_value_policy::reference_internal,
                             "Bid Trade")
      .def_property_readonly("askTrade", &Trade::GetAskTrade,
                             py::return_value_policy::reference_internal,
                             "Ask Trade")
      .def("__repr__", [](const Trade &trade) {
        return "BidTrade(orderID=" +
               std::to_string(trade.GetBidTrade().orderId_) +
               " ,Price=" + std::to_string(trade.GetBidTrade().price_) +
               " ,Quantity=" + std::to_string(trade.GetBidTrade().quantity_) +
               ") ," + "AskTrade(orderID=" +
               std::to_string(trade.GetAskTrade().orderId_) +
               " ,Price=" + std::to_string(trade.GetAskTrade().price_) +
               " ,Quantity=" + std::to_string(trade.GetAskTrade().quantity_) +
               ")";
      });
}
