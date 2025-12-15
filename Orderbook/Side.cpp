#include "include/Side.h"

std::string sideToString(Side side) {
  return side == Side::Buy ? "Buy" : "Sell";
}
