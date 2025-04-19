#include "../order/order.hpp"
#include "../trade/trade.hpp"
#include "../snowflake/snowflake.hpp"

#include <functional>
#include <map>
#include <mutex>
#include <queue>

class Orderbook {
private:
  // bid_orders holds all the orders on the bid side of the order book 
  // of a limit type
  std::map<Order::Price, Order::OrderPointers, std::greater<Order::Price>>
      bid_orders;
  // ask_orders holds all the orders on the ask side of the order book 
  // of a limit type
  std::map<Order::Price, Order::OrderPointers, std::less<Order::Price>>
      ask_orders;
  
  // market_buy_orders holds all the orders on the bid side of the order book
  // of a market type that were not filled immediately.
  std::queue<Order::OrderPointer> market_buy_orders;
  std::queue<Order::OrderPointer> market_sell_orders;
  mutable std::mutex mutex_;
  std::thread marketOrdersPruneThread_;
  SnowflakeGenerator trade_id_gen;
 

public:
  Orderbook() : trade_id_gen(SnowflakeGenerator(0)) {
    bid_orders = {};
    ask_orders = {};
    market_buy_orders = {};
    market_sell_orders = {};
  }
  int AddOrder(Order::OrderPointer order);
  int CancelOrder(Order::OrderId id);
  int HandleMarketOrder(Order::OrderPointer order);
  int HandleLimitOrder(Order::OrderPointer order);
  void MatchOrders();
  Trade MakeTrade(Order::OrderPointer o1, Order::OrderPointer o2);
};
