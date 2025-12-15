#pragma once
#include <string>

enum class OrderType {
  GoodTillCancel,
  FillAndKill,
  FillOrKill,
  Market,
  GoodForDay
};

std::string orderTypeToString(OrderType orderType) {
  switch (orderType) {
  case OrderType::FillAndKill:
    return "FillAndKill";
  case OrderType::FillOrKill:
    return "FillOrKill";
  case OrderType::GoodForDay:
    return "GoodForDay";
  case OrderType::GoodTillCancel:
    return "GoodTillCancel";
  case OrderType::Market:
    return "Market";
  }
}
