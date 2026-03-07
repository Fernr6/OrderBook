#include <iostream>
#include <vector>

#include "OrderBook.h"
int main() {

    OrderBook book;

    
    Order order1 {.order_id = 1, .price= 10000, .quantity = 50, .side = Side::BUY };
    Order order2 {.order_id = 2, .price= 10000, .quantity = 10, .side = Side::BUY };
    Order order3 {.order_id = 3, .price= 9950, .quantity = 100, .side = Side::BUY };
    Order order4 {.order_id = 4, .price= 10000, .quantity = 10, .side = Side::BUY };

    Order order5 {.order_id = 5, .price= 8000, .quantity = 70, .side = Side::SELL };
    Order order6 {.order_id = 6, .price= 7550, .quantity = 80, .side = Side::SELL };
    Order order7 {.order_id = 7, .price= 8950, .quantity = 10, .side = Side::SELL };
    Order order8 {.order_id = 8, .price= 7550, .quantity = 80, .side = Side::SELL };


    book.addOrder(order1);
    book.addOrder(order2);
    book.addOrder(order3);
    book.addOrder(order4);
    book.addOrder(order5);
    book.addOrder(order6);
    book.addOrder(order7);
    book.addOrder(order8);

    book.printBook();
    
    return 0;
}