#include <iostream>

#include "OrderBook.h"

void OrderBook::addOrder(Order inc_Order)
{

    if  (inc_Order.side == Side::BUY)
    {
        // MATCHING ORDERS
        while ( !asks.empty() && inc_Order.price >= asks.begin()->first )
        {
            // Grab sellers at best Ask price
            auto& currentSellersList = asks.begin()->second; 
            auto& firstBuyer = currentSellersList.front();

            // How many shares can we trade
            auto tradeQuantity = std::min(inc_Order.quantity, firstBuyer.quantity);

            std::cout << "TRADE EXECUTED: " << tradeQuantity  << " shares at PRICE: " << firstBuyer.price << std::endl;

            // Substract traded quantity from both orders
            inc_Order.quantity -= tradeQuantity;
            firstBuyer.quantity -= tradeQuantity;

            // Clean up empty orders
            if ( firstBuyer.quantity == 0 )
            {
                currentSellersList.erase(currentSellersList.begin());

                // If price level is empty, delete
                if ( currentSellersList.empty() )
                {
                    asks.erase(asks.begin());
                }

            }

            if ( inc_Order.quantity == 0 )
            {
                break;
            }
        }
        
        // Access vector stored at [inc_Order] in bids map, creates if missing
        if ( inc_Order.quantity > 0)
        {
            bids[inc_Order.price].push_back(inc_Order);
        }

    } else if (inc_Order.side == Side::SELL)
    {
        while ( !bids.empty() && inc_Order.price <= bids.begin()->first )
        {

            auto& currentBuyerList = bids.begin()->second; 
            auto& firstBuyer = currentBuyerList.front();

            auto tradeQuantity = std::min(inc_Order.quantity, firstBuyer.quantity);

            std::cout << "TRADE EXECUTED: " << tradeQuantity  << " shares at PRICE: " << firstBuyer.price << std::endl;

            inc_Order.quantity -= tradeQuantity;
            firstBuyer.quantity -= tradeQuantity;

            if ( firstBuyer.quantity == 0 )
            {
                currentBuyerList.erase(currentBuyerList.begin());
                if ( currentBuyerList.empty() )
                {
                    bids.erase(bids.begin());
                }

            }

            if ( inc_Order.quantity == 0 )
            {
                break;
            }
        }
        
        if ( inc_Order.quantity > 0)
        {
            asks[inc_Order.price].push_back(inc_Order);
        }
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