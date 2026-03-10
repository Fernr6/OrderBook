#include "Benchmark.h"
#include "OrderBook.h"
#include <iostream>
#include <chrono>
#include <random>

void runBenchmark(int num_orders)
{
    OrderBook book;

    std::mt19937 gen(42); 
    std::uniform_int_distribution<uint32_t> price_dist(9900, 10100); 
    std::uniform_int_distribution<uint32_t> quant_dist(10, 100);     
    std::uniform_int_distribution<int> side_dist(0, 1);              
    std::uniform_int_distribution<int> type_dist(0, 9);              

    std::cout << "Generating and processing " << num_orders << " orders..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_orders; ++i) 
    {
        Order order {
            .order_id = static_cast<uint64_t>(i),
            .price = price_dist(gen),
            .quantity = quant_dist(gen),
            .side = (side_dist(gen) == 0) ? Side::BUY : Side::SELL,
            .type = (type_dist(gen) == 0) ? OrderType::MARKET : OrderType::LIMIT
        };
        
        if (order.type == OrderType::MARKET) 
        {
            order.price = 0;
        }

        book.addOrder(order);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    std::cout << "========================================" << std::endl;
    std::cout << "Processed " << num_orders << " orders." << std::endl;
    std::cout << "Total Time: " << duration.count() << " ms" << std::endl;
    std::cout << "Throughput: " << (num_orders / (duration.count() / 1000.0)) << " orders per second" << std::endl;
    std::cout << "Average Latency: " << (duration.count() * 1000.0) / num_orders << " microseconds per order" << std::endl;
    std::cout << "========================================" << std::endl;
}