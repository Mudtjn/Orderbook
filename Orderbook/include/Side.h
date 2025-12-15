#pragma once
#include "Bindings.h"

enum class Side { Buy, Sell };

std::string sideToString(Side side) {
  return side == Side::Buy ? "Buy" : "Sell";
}
