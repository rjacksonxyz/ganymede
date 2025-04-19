#include "../src/orderbook/orderbook.hpp"

void makeBids(Orderbook &ob)
{
  for (int i = 0; i < 10; ++i)
  {
    auto op = std::make_shared<Order>(Side::Bid, 70 + i, OrderType::Limit,
                                      60.50 - i * 0.10);
    ob.AddOrder(op);
  }
}

void makeAsks(Orderbook &ob)
{
  for (int i = 0; i < 10; ++i)
  {
    auto op = std::make_shared<Order>(Side::Ask, 35 + i, OrderType::Limit,
                                      59.00 + i * 0.10);
    ob.AddOrder(op);
  }
}

int main()
{
  Orderbook ob = Orderbook();
  std::thread bid_thread(makeBids, std::ref(ob));
  std::thread ask_thread(makeAsks, std::ref(ob));
  bid_thread.join();
  ask_thread.join();
  ob.ShowOrders();
}
