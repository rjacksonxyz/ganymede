#include "../src/order/order.hpp"
#include "../src/orderbook/orderbook.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

class OrderbookLimitOrderTest : public ::testing::Test
{
protected:
  static const int ORDER_N = 20;
  int askQuantity = 0;
  int bidQuantity = 0;
  Orderbook ob;

  void SetUp() override
  {
    askQuantity = 0;
    bidQuantity = 0;
  }

public:
  void makeLimitBids()
  {
    std::list<Order> orders = getOrders();
    for (auto &op : orders)
    {
      op.SetSide(Side::Bid);
      ob.AddOrder(op);
      bidQuantity += op.GetQuantity();
    }
  }

  void makeLimitAsks()
  {
    std::list<Order> orders = getOrders();
    for (auto &op : orders)
    {
      op.SetSide(Side::Ask);
      ob.AddOrder(op);
      askQuantity += op.GetQuantity();
    }
  }

  std::list<Order> getOrders()
  {
    // construct 10 orders, all limit bid orders, increasing in price from
    // 59.50 to 60.50
    std::list<Order> orders;
    for (int i = 0; i < 10; ++i)
    {
      Price p = 59.50 + i * 0.10;
      Quantity q = 70 + i;
      Order op1(Side::Bid, q, OrderType::Limit, p);
      // pushing to front to test that the orderbook can handle it
      // if lower price orders are added first, they will be matched first
      // and the orderbook will still work correctly but not all orders will be matched ("overpriced" bids will match with "underpriced" asks)
      orders.push_front(op1);
    }
    return orders;
  }
};

TEST_F(OrderbookLimitOrderTest, LimitOrdersMatchCorrectlyAskFirst)
{
  makeLimitAsks();
  makeLimitBids();
  ob.ShowOrders();
  int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
  EXPECT_EQ(ob.GetTradeVolume(), expectedTradedQuantity);
}

TEST_F(OrderbookLimitOrderTest, LimitOrdersMatchCorrectlyBidFirst)
{
  makeLimitBids();
  makeLimitAsks();
  ob.ShowOrders();
  int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
  EXPECT_EQ(ob.GetTradeVolume(), expectedTradedQuantity);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
