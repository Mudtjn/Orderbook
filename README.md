levels - price levels in orderbook

# Concepts
1. Resource Allocation is Initialization (RAII)
   1. Resource Acquisition (eg. allocation of memory, opening file, locking mutex) occurs during object construction. 
   2. Resource release happens automatically during object destruction
   3. As long as object is alive, the resource is guaranteed to be available. 
   4. **Advantages**
      1. No resource leaks
      2. Exception safety
      3. Deterministic cleanup
2. `std::memory_order` [cppreference](https://en.cppreference.com/w/cpp/atomic/memory_order.html)
   1. specifies how memory accesses, including regular, non-atomic memory accesses, are to be ordered an atomic operation. 