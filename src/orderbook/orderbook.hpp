#pragma once

#include "../order/order.hpp"
#include "../snowflake/snowflake.hpp"
#include "../trade/trade.hpp"

#include "../order/order.hpp"
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <mutex>
#include <queue>

class Orderbook {
  using Trades = std::vector<Trade>;
  template <typename Comparator>
  using OrderMap = std::map<Price, OrderPointers, Comparator>;
  using AskOrders = OrderMap<std::less<Price>>;
  using BidOrders = OrderMap<std::greater<Price>>;

private:
  struct OrderEntry {
    OrderPointer order_{nullptr};
    OrderPointers::iterator location_;
  };
  // unordered map of all orders, stored by order ID.
  std::unordered_map<OrderId, OrderEntry> orders_;
  // bid_orders holds all the orders on the bid side of the order book
  // of a limit type.
  BidOrders bid_orders;
  // ask_orders holds all the orders on the ask side of the order book
  // of a limit type.
  AskOrders ask_orders;

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
  Trades trades;
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
  bool CanMatchLimitOrders();
  void MatchMarketOrders(OrderPointer market_ask, BidOrders &non_market_orders);
  void MatchMarketOrders(OrderPointer market_bid, AskOrders &non_market_orders);
  void MatchLimitOrders(Trades &t);
  Quantity GetTradeVolume() {
    std::scoped_lock lock{mutex_};
    Quantity trade_volume = 0;
    for (auto trade : trades) {
      trade_volume += trade.GetQuantity();
    }
    return trade_volume;
  }
  Trades GetTrades() const {
    std::scoped_lock lock{mutex_};
    return trades;
  }
  short GetSideCount() const {
    return (short)!bid_orders.empty() + (short)!ask_orders.empty() +
           (short)!market_bid_orders.empty() +
           (short)!market_ask_orders.empty();
  }
};
