#pragma once

#include <map>
#include <vector>
#include <iostream>

#include "Order.h"

class OrderBook
{
private:
    
    // BIDS = sort High-Low (std::greater)
    // Key = Price, Value = vector of orders  
    std::map<uint32_t, std::vector<Order>, std::greater<uint32_t>> bids;
    
    // ASKS = sort Low-High (std::less)
    std::map<uint32_t, std::vector<Order>, std::less<uint32_t>> asks;

    // TRADE LEDGER
    std::vector<Trade> tradeHistory;


    
public:

    // const Order& to not copy/modify (making a copy for simplicity, will optimize later on)
    void addOrder(Order _order);

    // const = will not modify anythign
    void printBook() const;

    void printTape() const;
};



