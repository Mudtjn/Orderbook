#pragma once
#include <string>

enum class OrderType {
  GoodTillCancel,
  FillAndKill,
  FillOrKill,
  Market,
  GoodForDay
};

std::string orderTypeToString(OrderType orderType);
