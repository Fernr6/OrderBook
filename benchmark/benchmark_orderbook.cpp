#include "../include/OrderBook.h"
#include <benchmark/benchmark.h>
#include <cstdint>

// Benchmark 1: Adding millions of non-matching limit orders
static void BM_AddLimitOrders(benchmark::State &state) {
  const int num_orders = state.range(0);

  for (auto _ : state) {
    state.PauseTiming();

    // 1. Open a manual scope block
    {
      OrderBook book;
      state.ResumeTiming(); // Start the clock

      // Do the work
      for (int i = 0; i < num_orders; ++i) {
        book.addOrder({.order_id = static_cast<uint64_t>(i),
                       .price = static_cast<uint32_t>(100 + (i % 1000)),
                       .quantity = 10,
                       .side = Side::BUY,
                       .type = OrderType::LIMIT});
      }

      state.PauseTiming(); // Stop the clock
    } // 2. 'book' is destroyed right here while the clock is safely paused!

    state.ResumeTiming(); // 3. Turn the clock back on for Google Benchmark's
                          // loop manager
  }
  state.SetItemsProcessed(state.iterations() * num_orders);
}

// Benchmark 2: Sweeping millions of resting orders with a massive market order
static void BM_MatchMarketOrder(benchmark::State &state) {
  const int num_resting_orders = state.range(0);

  for (auto _ : state) {
    state.PauseTiming();

    { // Open scope
      OrderBook book;
      for (int i = 0; i < num_resting_orders; ++i) {
        book.addOrder({.order_id = static_cast<uint64_t>(i),
                       .price = 150,
                       .quantity = 10,
                       .side = Side::SELL,
                       .type = OrderType::LIMIT});
      }

      state.ResumeTiming(); // Start clock for the matching logic

      book.addOrder({.order_id = 99999999,
                     .price = 0,
                     .quantity = static_cast<uint32_t>(num_resting_orders * 10),
                     .side = Side::BUY,
                     .type = OrderType::MARKET});

      state.PauseTiming(); // Stop clock
    } // 'book' is destroyed here

    state.ResumeTiming(); // Restart clock for GB
  }
  state.SetItemsProcessed(state.iterations() * (num_resting_orders + 1));
}
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
