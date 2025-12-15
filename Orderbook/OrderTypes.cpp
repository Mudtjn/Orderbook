#include "include/OrderTypes.h"

std::string orderTypeToString(OrderType orderType) {
  switch (orderType) {
  case OrderType::FillAndKill:
    return "FillAndKill";
  case OrderType::FillOrKill:
    return "FillOrKill";
  case OrderType::GoodTillCancel:
    return "GoodTillCancel";
  case OrderType::Market:
    return "Market";
  case OrderType::GoodForDay:
    return "GoodForDay";
  }
}
