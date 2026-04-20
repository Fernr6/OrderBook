#pragma once

#include <cstdint>
#include <deque>
#include <vector>

#include "Order.h"

class OrderBook {
private:
  std::vector<std::deque<Order>> asks;

  std::vector<std::deque<Order>> bids;

  uint32_t best_ask;
  uint32_t best_bid;
  uint32_t max_price;

public:
  explicit OrderBook(uint32_t max_price_ticks = 1000000);

  void addOrder(Order _order);

  // const = will not modify anythign
  void printBook() const;
};
