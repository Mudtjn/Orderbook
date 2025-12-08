#include "include/BindSide.h"

void bindSide(py::module &m) {
  py::enum_<Side>(m, "Side", "Order Side: Buy or Sell")
      .value("Buy", Side::Buy)
      .value("Sell", Side::Sell)
      .export_values();
}
