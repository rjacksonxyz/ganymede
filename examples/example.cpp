#include "../src/orderbook/orderbook.hpp"

int main()
{
    Orderbook ob = Orderbook();
    // Generate 10 bid orders
    for (int i = 0; i < 10; ++i)
    {
        auto op = std::make_shared<Order>(Side::Bid, 70 + i, OrderType::Limit, 60.50 - i * 0.10);
        ob.AddOrder(op);
    }
    ob.ShowOrders();

    // Generate 10 ask orders
    for (int i = 0; i < 10; ++i)
    {
        auto op = std::make_shared<Order>(Side::Ask, 35 + i, OrderType::Limit, 59.00 + i * 0.10);
        ob.AddOrder(op);
    }
    ob.ShowOrders();

    ob.CancelOrder(9);
    ob.ShowOrders();
}
