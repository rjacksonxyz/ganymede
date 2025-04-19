#include "../order/order.hpp"
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include "../snowflake/snowflake.hpp"

class Trade {
    Order::Quantity quantity;
    Order::Price price;
    Order::OrderId buy_id;
    Order::OrderId ask_id;
    std::chrono::nanoseconds timestamp;
    int64_t id;
    
public:
    Trade() = delete;
    Trade(Order::OrderPointer bid, Order::OrderPointer ask, Order::OrderType type_, int64_t trade_id)
        : buy_id(bid->GetId()),
          ask_id(ask->GetId()),
          timestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(
              std::chrono::system_clock::now().time_since_epoch())),
          id(trade_id)
    {
        quantity = bid->GetQuantity() > ask->GetQuantity() ? bid->GetQuantity() : ask->GetQuantity();
    }
};
