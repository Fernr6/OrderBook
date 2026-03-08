#pragma once // Tells the compiler to only include this file once per build
#include <cstdint>

enum class Side 
{
    BUY,
    SELL
};

enum OrderType
{
    LIMIT,
    MARKET 
};

struct Order 
{
    uint64_t order_id;
    uint32_t price;     
    uint32_t quantity;
    Side side;
    OrderType type;
};

struct Trade
{
    uint64_t buyer_id;
    uint64_t seller_id;
    uint32_t price;
    uint32_t quantity;
};
