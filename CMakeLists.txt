cmake_minimum_required(VERSION 3.14)
project(OrderBookEngine)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- OrderBook logic ---
add_library(OrderBookLib src/OrderBook.cpp)
target_include_directories(OrderBookLib PUBLIC include)

# --- Main Executable ---
add_executable(OrderBookMain main.cpp)
target_link_libraries(OrderBookMain PRIVATE OrderBookLib)

# --- Google Benchmark ---
include(FetchContent)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.8.3 
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark tests" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Disable benchmark install" FORCE)
FetchContent_MakeAvailable(benchmark)
find_package(Threads REQUIRED)

# --- Benchmark ---
add_executable(OrderBookBenchmark benchmark/benchmark_orderbook.cpp)

target_link_libraries(OrderBookBenchmark PRIVATE 
    OrderBookLib 
    benchmark::benchmark 
    Threads::Threads
)
