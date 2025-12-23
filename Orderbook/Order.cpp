#include "include/Order.h"
#include "include/OrderTypes.h"
#include "include/Usings.h"
#include <atomic>

snowflake_t Order::uuid;

Order::Order(OrderType orderType, Side side, Price price, Quantity quantity)
    : orderType_{orderType}, side_{side}, price_{price},
      initialQuantity_{quantity}, remainingQuantity_{quantity} {

  if (isInitialOrder.load(std::memory_order_acquire) == 0) {
    uuid.init(1, 1);
    isInitialOrder.store(true);
  }
  orderId_ = uuid.nextid();
}

Order::Order(Side side, Quantity quantity)
    : orderType_{OrderType::Market}, side_{side},
      price_{Constants::InvalidPrice}, initialQuantity_{quantity},
      remainingQuantity_{quantity} {
  if (isInitialOrder.load(std::memory_order_acquire) == 0) {
    uuid.init(1, 1);
    isInitialOrder.store(true);
  }
  orderId_ = uuid.nextid();
}
