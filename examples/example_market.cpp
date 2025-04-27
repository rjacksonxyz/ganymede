#include "../src/orderbook/orderbook.hpp"

void makeBids(Orderbook &ob);
void makeAsks(Orderbook &ob);

const int ORDER_N = 20;
int askQuantity = 0;
int bidQuantity = 0;

int main()
{
  Orderbook ob = Orderbook();
  std::thread bid_thread(makeBids, std::ref(ob));
  bid_thread.join();
  ob.ShowOrders();
  std::thread ask_thread(makeAsks, std::ref(ob));
  ask_thread.join();
  int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
  std::cout << "Expected Traded Quantity: " << expectedTradedQuantity << std::endl;
  std::cout << "Actual Traded Quantity: " << ob.GetTradeVolume() << std::endl;
}

void makeAsks(Orderbook &ob)
{
  for (int i = 0; i < ORDER_N / 2; ++i)
  {
    Quantity q = 35 + i;
    Price p = 59.50 + i * 0.10;
    Order op1(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op1);
    Order op2(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op2);
    askQuantity += q * 2;
  }
}

void makeBids(Orderbook &ob)
{
  for (int i = 0; i < ORDER_N; ++i)
  {
    Quantity q = 70 + i;
    Order op1(Side::Bid, q, OrderType::Market, 0);
    ob.AddOrder(op1);
    bidQuantity += q;
  }
}
