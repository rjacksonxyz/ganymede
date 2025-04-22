#include "../src/orderbook/orderbook.hpp"

void makeBids(Orderbook &ob);
void makeAsks(Orderbook &ob);

int main()
{
  Orderbook ob = Orderbook();
  std::thread bid_thread(makeBids, std::ref(ob));
  std::thread ask_thread(makeAsks, std::ref(ob));
  bid_thread.join();
  ask_thread.join();
  ob.ShowOrders();
}

void makeAsks(Orderbook &ob)
{
  for (int i = 0; i < 10; ++i)
  {
    Quantity q = 35 + i;
    Price p = 59.50 + i * 0.10;
    auto op1 = std::make_shared<Order>(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op1);
    auto op2 = std::make_shared<Order>(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op2);
  }
}

void makeBids(Orderbook &ob)
{
  for (int i = 0; i < 10; ++i)
  {
    Quantity q = 70 + i;
    Price p = 60.50 - i * 0.10;
    auto op1 = std::make_shared<Order>(Side::Bid, q, OrderType::Limit, p);
    ob.AddOrder(op1);
    auto op2 = std::make_shared<Order>(Side::Bid, q, OrderType::Limit, p);
    ob.AddOrder(op2);
  }
}