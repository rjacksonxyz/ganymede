#include "../src/orderbook/orderbook.hpp"

void makeBids(Orderbook &ob);
void makeAsks(Orderbook &ob);

const int ORDER_N = 20;

int main()
{
  Orderbook ob = Orderbook();
  std::thread bid_thread(makeBids, std::ref(ob));
  bid_thread.join();
  ob.ShowOrders();
  std::thread ask_thread(makeAsks, std::ref(ob));
  ask_thread.join();
  ob.ShowOrders();
}

void makeAsks(Orderbook &ob)
{
  for (int i = 0; i < ORDER_N / 2; ++i)
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
  for (int i = 0; i < ORDER_N; ++i)
  {
    Quantity q = 70 + i;
    auto op1 = std::make_shared<Order>(Side::Bid, q, OrderType::Market, 0);
    ob.AddOrder(op1);
  }
}
