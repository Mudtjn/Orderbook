#include "include/BindOrderTypes.h"
#include "include/OrderTypes.h"

void bindOrderTypes(py::module &m) {
  py::enum_<OrderType>(m, "OrderType", "OrderType: type of Order")
      .value("FillAndKill", OrderType::FillAndKill)
      .value("FillOrKill", OrderType::FillOrKill)
      .value("GoodForDay", OrderType::GoodForDay)
      .value("GoodTillCancel", OrderType::GoodTillCancel)
      .value("Market", OrderType::Market)
      .export_values();
}
