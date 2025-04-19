#include "../order/order.hpp"
#include "../trade/trade.hpp"
#include "../snowflake/snowflake.hpp"

#include <functional>
#include <map>
#include <mutex>
#include <queue>

class Orderbook
{
private:
  // bid_orders holds all the orders on the bid side of the order book
  // of a limit type
  std::map<Price, OrderPointers, std::greater<Price>>
      bid_orders;
  // ask_orders holds all the orders on the ask side of the order book
  // of a limit type
  std::map<Price, OrderPointers, std::less<Price>>
      ask_orders;

  // market_buy_orders holds all the orders on the bid side of the order book
  // of a market type that were not filled immediately.
  std::queue<OrderPointer> market_buy_orders;
  std::queue<OrderPointer> market_sell_orders;
  mutable std::mutex mutex_;
  std::thread marketOrdersPruneThread_;
  SnowflakeGenerator trade_id_gen;

public:
  Orderbook() : trade_id_gen(SnowflakeGenerator(0))
  {
    bid_orders = {};
    ask_orders = {};
    market_buy_orders = {};
    market_sell_orders = {};
  }
  int AddOrder(OrderPointer order);
  int CancelOrder(OrderId id);
  int HandleMarketOrder(OrderPointer order);
  int HandleLimitOrder(OrderPointer order);
  void ShowOrders();
  void MatchOrders();
  Trade MakeTrade(OrderPointer o1, OrderPointer o2);
};
