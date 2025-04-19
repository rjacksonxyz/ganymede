#pragma once
#ifndef GNYD_TRADE
#define GNYD_TRADE
#include "../order/order.hpp"
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "../snowflake/snowflake.hpp"

class Trade
{
    Quantity quantity;
    Price price;
    OrderId bid_id;
    OrderId ask_id;
    std::chrono::nanoseconds timestamp;
    int64_t id;

public:
    Trade() = delete;
    Trade(OrderPointer bid, OrderPointer ask, Quantity quantity_, Price price_, int64_t trade_id)
        : bid_id(bid->GetId()),
          ask_id(ask->GetId()),
          timestamp(std::chrono::duration_cast<std::chrono::nanoseconds>(
              std::chrono::system_clock::now().time_since_epoch())),
          id(trade_id),
          quantity(quantity_),
          price(price_)
    {
    }
    friend std::ostream &operator<<(std::ostream &os, const Trade &t)
    {
        os << "Trade ID: " << t.id << ", Bid ID: " << t.bid_id << ", Ask ID: " << t.ask_id << ", Quantity: " << t.quantity << ", Price: " << t.price << ", Timestamp: " << t.timestamp.count() << '\n';
        return os;
    }
};
#endif