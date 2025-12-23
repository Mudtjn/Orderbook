#pragma once
#include "snowflake.hpp"
#include <iostream>
#include <stdint.h>
#include <vector>

using Price = int32_t;
using Quantity = uint32_t;
using OrderId = uint64_t;
using OrderIds = std::vector<OrderId>;
using snowflake_t = snowflake<1534832906275L, std::mutex>;
