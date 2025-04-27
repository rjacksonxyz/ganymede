#include "../src/orderbook/orderbook.hpp"

void makeBids(Orderbook &ob);
void makeAsks(Orderbook &ob);

int main() {
  Orderbook ob = Orderbook();
  makeBids(ob);
  makeAsks(ob);
  ob.ShowOrders();
}

void makeAsks(Orderbook &ob) {
  for (int i = 0; i < 10; ++i) {
    Quantity q = 35 + i;
    Price p = 59.50 + i * 0.10;
    Order op1(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op1);
    Order op2(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op2);
  }
}

void makeBids(Orderbook &ob) {
  for (int i = 0; i < 10; ++i) {
    Quantity q = 70 + i;
    Price p = 60.50 - i * 0.10;
    Order op1(Side::Bid, q, OrderType::Limit, p);
    ob.AddOrder(op1);
    Order op2(Side::Bid, q, OrderType::Limit, p);
    ob.AddOrder(op2);
  }
}
