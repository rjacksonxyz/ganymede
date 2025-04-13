#include "../order/order.hpp"
#include <functional>
#include <map>

class OrderBook {
private:
  std::map<Order::Price, Order::OrdersPointer, std::greater<Order::Price>>
      buy_orders;
  std::map<Order::Price, Order::OrdersPointer, std::less<Order::Price>>
      sell_orders;
};
