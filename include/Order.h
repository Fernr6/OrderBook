#pragma once // Tells the compiler to only include this file once per build
#include <cstdint>

enum class Side {
    BUY,
    SELL
};

struct Order {
    uint64_t order_id;
    uint32_t price;     
    uint32_t quantity;
    Side side;
};