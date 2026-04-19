#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <map>

#include "Order.h"

class OrderBook {
private:
  std::map<uint32_t, std::deque<Order>> asks;

  std::map<uint32_t, std::deque<Order>, std::greater<double>> bids;

public:
  // const Order& to not copy/modify (making a copy for simplicity, will
  // optimize later on)
  void addOrder(Order _order);

  // const = will not modify anythign
  void printBook() const;
};
