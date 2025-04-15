#include "../order/order.hpp"
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

class Trade {
    
    Order::Quantity quantity;
    Order::Price price;
    Order::OrderId buy_id;
    Order::OrderId sell_id;
    std::chrono::nanoseconds timestamp;
    int64_t id ; 

public:
    Trade(); //TODO: Implement constructor
};