#include "../include/OrderBook.h"
#include <benchmark/benchmark.h>
#include <cstdint> // Added to resolve "undeclared identifier" for uint32_t/uint64_t

// Benchmark 1: Adding millions of non-matching limit orders
static void BM_AddLimitOrders(benchmark::State &state) {
  const int num_orders = state.range(0);

  for (auto _ : state) {
    // Pause timing to create a fresh order book without counting it in the
    // benchmark time
    state.PauseTiming();
    OrderBook book;
    state.ResumeTiming();

    for (int i = 0; i < num_orders; ++i) {
      book.addOrder(
          {.order_id = static_cast<uint64_t>(i),
           // Modulo spreads the orders across 1000 different price levels
           .price = static_cast<uint32_t>(100 + (i % 1000)),
           .quantity = 10,
           .side = Side::BUY,
           .type = OrderType::LIMIT});
    }
  }
  // This tells Google Benchmark to calculate our orders processed per second
  state.SetItemsProcessed(state.iterations() * num_orders);
}

// Benchmark 2: Sweeping millions of resting orders with a massive market order
static void BM_MatchMarketOrder(benchmark::State &state) {
  const int num_resting_orders = state.range(0);

  for (auto _ : state) {
    state.PauseTiming();
    OrderBook book;
    // Pre-fill the book with resting SELL limit orders at the SAME price level
    for (int i = 0; i < num_resting_orders; ++i) {
      book.addOrder({.order_id = static_cast<uint64_t>(i),
                     .price = 150,
                     .quantity = 10,
                     .side = Side::SELL,
                     .type = OrderType::LIMIT});
    }
    state.ResumeTiming();

    // Send ONE massive MARKET BUY order that sweeps all resting sellers
    book.addOrder({.order_id = 99999999,
                   .price = 0,
                   .quantity = static_cast<uint32_t>(num_resting_orders * 10),
                   .side = Side::BUY,
                   .type = OrderType::MARKET});
  }
  // We count the setup limit orders + the 1 market order as the total items
  // processed
  state.SetItemsProcessed(state.iterations() * (num_resting_orders + 1));
}

// Register the benchmarks with 1M, 10M, and 20M arguments.
// We set the time unit to Milliseconds so the output is easier to read.
BENCHMARK(BM_AddLimitOrders)
    ->Arg(1000000)
    ->Arg(10000000)
    ->Arg(20000000)
    ->Unit(benchmark::kMillisecond);

BENCHMARK(BM_MatchMarketOrder)
    ->Arg(1000000)
    ->Arg(10000000)
    ->Arg(20000000)
    ->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
