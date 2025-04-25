#include "../src/order/order.hpp"
#include "../src/orderbook/orderbook.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

class OrderbookLimitOrderTest : public ::testing::Test {
protected:
  static const int ORDER_N = 20;
  int askQuantity = 0;
  int bidQuantity = 0;
  Orderbook ob;

  void SetUp() override {
    askQuantity = 0;
    bidQuantity = 0;
  }

public:
  void makeLimitBids() {
    OrderPointers orders;
    getOrders(orders);
    for (auto &op : orders) {
      op->SetSide(Side::Bid);
      ob.AddOrder(op);
      bidQuantity += op->GetQuantity();
    }
  }

  void makeLimitAsks() {
    OrderPointers orders;
    getOrders(orders);
    for (auto &op : orders) {
      op->SetSide(Side::Ask);
      ob.AddOrder(op);
      askQuantity += op->GetQuantity();
    }
  }

  void getOrders(OrderPointers &orders) {
    // construct 10 orders, all limit bid orders, increasing in price from
    // 59.50 to 60.50
    for (int i = 0; i < 10; ++i) {
      Price p = 59.50 + i * 0.10;
      Quantity q = 70 + i;
      auto op1 = std::make_shared<Order>(Side::Bid, q, OrderType::Limit, p);
      orders.push_back(op1);
    }
  }
};

TEST_F(OrderbookLimitOrderTest, LimitOrdersMatchCorrectly) {
  std::thread bid_thread(&OrderbookLimitOrderTest::makeLimitBids, this);
  std::thread ask_thread(&OrderbookLimitOrderTest::makeLimitAsks, this);

  ask_thread.join();
  bid_thread.join();

  int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
  EXPECT_EQ(ob.GetTradeVolume(), expectedTradedQuantity);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
