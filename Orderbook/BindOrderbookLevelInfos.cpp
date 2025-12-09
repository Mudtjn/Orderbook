#include "include/BindOrderbookLevelInfos.h"

void bindOrderbookLevelInfos(py::module &m) {
  py::class_<OrderbookLevelInfos>(m, "OrderbookLevelInfos",
                                  "Bid and ask levels in the orderbook")
      .def(py::init<const LevelInfos &, const LevelInfos &>(), py::arg("bids"),
           py::arg("asks"), "Construct with bid and ask levels")

      // Use getter methods
      .def("GetBids", &OrderbookLevelInfos::GetBids,
           py::return_value_policy::reference_internal, // Important: reference
                                                        // to internal data
           "Get bid levels (sorted from highest to lowest price)")

      .def("GetAsks", &OrderbookLevelInfos::GetAsks,
           py::return_value_policy::reference_internal, // Important: reference
                                                        // to internal data
           "Get ask levels (sorted from lowest to highest price)")

      // Python properties for cleaner access
      .def_property_readonly("Bids", &OrderbookLevelInfos::GetBids,
                             py::return_value_policy::reference_internal,
                             "Bid levels")

      .def_property_readonly("Asks", &OrderbookLevelInfos::GetAsks,
                             py::return_value_policy::reference_internal,
                             "Ask levels")

      // String representation
      .def("__repr__",
           [](const OrderbookLevelInfos &infos) {
             return "OrderbookLevelInfos(totalBids=" +
                    std::to_string(infos.GetBids().size()) +
                    ", totalAsks=" + std::to_string(infos.GetAsks().size()) +
                    ")";
           })

      .def("__str__", [](const OrderbookLevelInfos &infos) {
        std::string result = "Orderbook Levels:\n Bids: {";
        for (const auto &bid : infos.GetBids()) {
          result += "\n  (Price:" + std::to_string(bid.price) +
                    ", Qty:" + std::to_string(bid.quantity) + "),";
        }
        result += "\n },\n Asks: {";
        for (const auto &ask : infos.GetAsks()) {
          result += "\n  (Price:" + std::to_string(ask.price) +
                    ", Qty:" + std::to_string(ask.quantity) + "),";
        }
        result += "\n }\n}";
        return result;
      });
}
