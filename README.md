levels - price levels in orderbook

# Concepts

## C++ related concepts

   1. Resource Allocation is Initialization (RAII)
      1. Resource Acquisition (eg. allocation of memory, opening file, locking mutex) occurs during object construction.
      2. Resource release happens automatically during object destruction
      2. As long as object is alive, the resource is guaranteed to be available.
      3. **Advantages**
         1. No resource leaks
         2. Exception safety
         3. Deterministic cleanup
   2. `std::memory_order` [cppreference](https://en.cppreference.com/w/cpp/atomic/memory_order.html)
      1. specifies how memory accesses, including regular, non-atomic memory accesses, are to be ordered an atomic operation.

## Orderbook related concepts

   1. `FillAndKill` v/s `FillOrKill` orders
      1. `FillAndKill` states to complete as much order to be completed as possible and kill unfulfilled.
      2. `FillOrKill` states to either complete the order, or do not execute it all.

Notes:

1. To build Orderbook:

```bash
c++ -O3 -Wall -shared -std=c++20 -fPIC $(python3 -m pybind11 --includes) Side.cpp OrderTypes.cpp BindOrder.cpp BindOrderModify.cpp BindOrderbook.cpp BindOrderbookLevelInfos.cpp BindLevelInfo.cpp BindOrderTypes.cpp BindSide.cpp Orderbook.cpp -o orderbook$(python3-config --extension-suffix)
```
