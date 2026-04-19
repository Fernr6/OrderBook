cmake_minimum_required(VERSION 3.14)
project(OrderBookEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Library target for the OrderBook logic ---
add_library(OrderBookLib src/OrderBook.cpp)
target_include_directories(OrderBookLib PUBLIC include)

# --- Main Executable ---
add_executable(OrderBookMain main.cpp)
target_link_libraries(OrderBookMain PRIVATE OrderBookLib)

# --- Google Benchmark Setup ---
include(FetchContent)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3 # Use a recent stable release
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install" FORCE)
FetchContent_MakeAvailable(benchmark)

# --- Find System Threads (Crucial for Ubuntu/Linux) ---
find_package(Threads REQUIRED)

# --- Benchmark Executable ---
add_executable(OrderBookBenchmark benchmark/benchmark_orderbook.cpp)

# Link OrderBookLib, Google Benchmark, AND system threads
target_link_libraries(OrderBookBenchmark PRIVATE 
    OrderBookLib 
    benchmark::benchmark 
    Threads::Threads
)
