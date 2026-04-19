#include <algorithm>
#include <iostream>

#include "OrderBook.h"

OrderBook::OrderBook(uint32_t max_price_ticks)
    : asks(max_price_ticks), bids(max_price_ticks), best_bid(0),
      best_ask(max_price_ticks), max_price(max_price_ticks) {}

void OrderBook::addOrder(Order inc_Order) {
  if (inc_Order.price >= max_price && inc_Order.type == OrderType::LIMIT) {
    std::cerr << "Order rejected: Price exceeds maximum book size" << std::endl;
    return;
  }

  if (inc_Order.side == Side::BUY) {
    while (inc_Order.quantity > 0 && best_ask < max_price) {
      if (inc_Order.type == OrderType::LIMIT && inc_Order.price < best_ask) {
        break;
      }

      auto &currentSellersList = asks[best_ask];

      if (currentSellersList.empty()) {
        best_ask++;
        continue;
      }

      auto &firstSeller = currentSellersList.front();
      auto tradeQuantity = std::min(inc_Order.quantity, firstSeller.quantity);

      Trade newTrade{.buyer_id = inc_Order.order_id,
                     .seller_id = firstSeller.order_id,
                     .price = firstSeller.price,
                     .quantity = tradeQuantity};

      inc_Order.quantity -= tradeQuantity;
      firstSeller.quantity -= tradeQuantity;

      if (firstSeller.quantity == 0) {
        currentSellersList.pop_front();
        if (currentSellersList.empty()) {
          best_ask++;
        }
      }
    }
    if (inc_Order.quantity > 0 && inc_Order.type == OrderType::LIMIT) {
      bids[inc_Order.price].push_back(std::move(inc_Order));
      best_bid = std::max(best_bid, inc_Order.price);
    }
  } else if (inc_Order.side == Side::SELL) {
    while (inc_Order.quantity > 0 && best_bid > 0 ||
           (best_bid == 0 && !bids[0].empty())) {
      if (inc_Order.type == OrderType::LIMIT && inc_Order.price > best_bid) {
        break;
      }

      auto &currentBuyerList = bids[best_bid];

      if (currentBuyerList.empty()) {
        if (best_bid == 0)
          break;
        best_bid--;
        continue;
      }

      auto &firstBuyer = currentBuyerList.front();
      auto tradeQuantity = std::min(inc_Order.quantity, firstBuyer.quantity);

      Trade newTrade{.buyer_id = firstBuyer.order_id,
                     .seller_id = inc_Order.order_id,
                     .price = firstBuyer.price,
                     .quantity = tradeQuantity};

      inc_Order.quantity -= tradeQuantity;
      firstBuyer.quantity -= tradeQuantity;

      if (firstBuyer.quantity == 0) {
        currentBuyerList.pop_front();
        if (currentBuyerList.empty()) {
          if (best_bid > 0)
            best_bid--;
        }
      }
    }

    if (inc_Order.quantity > 0 && inc_Order.type == OrderType::LIMIT) {
      asks[inc_Order.price].push_back(std::move(inc_Order));
      best_ask = std::min(best_ask, inc_Order.price);
    }
  }
}
