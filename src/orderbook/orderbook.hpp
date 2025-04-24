#pragma once

#include "../order/order.hpp"
#include "../snowflake/snowflake.hpp"
#include "../trade/trade.hpp"

#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <mutex>
#include <queue>

class Orderbook {
  using Trades = std::vector<Trade>;

private:
  struct OrderEntry {
    OrderPointer order_{nullptr};
    OrderPointers::iterator location_;
  };
  // unordered map of all orders, stored by order ID.
  std::unordered_map<OrderId, OrderEntry> orders_;
  // bid_orders holds all the orders on the bid side of the order book
  // of a limit type.
  std::map<Price, OrderPointers, std::greater<Price>> bid_orders;
  // ask_orders holds all the orders on the ask side of the order book
  // of a limit type.
  std::map<Price, OrderPointers, std::less<Price>> ask_orders;

  // Price at which latest trade was executed.
  // TODO: Update matching logic to update last price of instrument.
  // Q: does it make sense to break out price reporting function to a separate
  // reporting service that consumes trade outputs?
  Price last_price;
  // market_buy_orders holds all the orders on the bid side of the order book
  // of a market type that were not filled immediately.
  std::list<OrderPointer> market_bid_orders;
  std::list<OrderPointer> market_ask_orders;
  std::thread marketOrdersPruneThread_;
  SnowflakeGenerator trade_id_gen;

  mutable std::mutex mutex_;

public:
  Orderbook() : trade_id_gen(SnowflakeGenerator(0)) {
    bid_orders = {};
    ask_orders = {};
    market_bid_orders = {};
    market_ask_orders = {};
  }
  int AddOrder(OrderPointer order);
  void CancelOrder(OrderId id);
  void CancelOrderInternal(OrderId orderId);
  OrderPointers::iterator HandleMarketOrder(OrderPointer order);
  OrderPointers::iterator HandleLimitOrder(OrderPointer order);
  void ShowOrders();
  void MatchOrders();
  void MatchMarketOrders(Trades &t);
  void MatchLimitOrders(Trades &t);
};
