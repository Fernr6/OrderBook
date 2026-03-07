#include <iostream>

#include "OrderBook.h"

void OrderBook::addOrder(const Order& _order)
{
    auto currentPrice = _order.price;

    if  (_order.side == Side::BUY)
    {
        // Access vector stored at [currentPrice] in bids map, creates if missing
        bids[currentPrice].push_back(_order);

    } else if (_order.side == Side::SELL)
    {
        // Access vector stored at [currentPrice] in asks map
        asks[currentPrice].push_back(_order);
    }
}

void OrderBook::printBook() const
{
    std::cout << "===== ASKS =====" << std::endl ;
    for (const auto& price_level : asks)
    {
        auto price = price_level.first;
        const auto& order_list = price_level.second;
        std::cout << "Price: " << price << " || Orders at this level: " << order_list.size() << std::endl;
    }

    std::cout << "===== BIDS =====" << std::endl ;
    for (const auto& price_level : bids)
    {
        auto price = price_level.first;
        const auto& order_list = price_level.second;
        std::cout << "Price: " << price << " || Orders at this level: " << order_list.size() << std::endl;
    }
}