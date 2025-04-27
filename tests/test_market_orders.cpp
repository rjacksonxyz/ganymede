#include "../src/orderbook/orderbook.hpp"
#include <algorithm>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

class OrderbookMarketOrderTest : public ::testing::Test {
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
    std::list<Order> orders = getOrders(OrderType::Limit);
    for (auto &op : orders) {
      op.SetSide(Side::Bid);
      ob.AddOrder(op);
      bidQuantity += op.GetQuantity();
    }
  }

  void makeMarketBids() {
    std::list<Order> orders = getOrders(OrderType::Market);
    for (auto &op : orders) {
      op.SetSide(Side::Bid);
      ob.AddOrder(op);
      bidQuantity += op.GetQuantity();
    }
  }

  void makeLimitAsks() {
    std::list<Order> orders = getOrders(OrderType::Limit);
    for (auto &op : orders) {
      op.SetSide(Side::Ask);
      ob.AddOrder(op);
      askQuantity += op.GetQuantity();
    }
  }

  void makeMarketAsks() {
    std::list<Order> orders = getOrders(OrderType::Market);
    for (auto &op : orders) {
      op.SetSide(Side::Ask);
      ob.AddOrder(op);
      askQuantity += op.GetQuantity();
    }
  }

  std::list<Order> getOrders(OrderType orderType) {
    // construct 10 orders, all limit bid orders, increasing in price from
    // 59.50 to 60.50
    std::list<Order> orders;
    for (int i = 0; i < 10; ++i) {
      Price p = 59.50 + i * 0.10;
      if (orderType == OrderType::Market) {
        p = 0; // market orders don't have a price
      }
      Quantity q = 70 + i;
      Order op1(Side::Bid, q, orderType, p);
      // pushing to front for test logic.
      // if lower price orders are added first, they will be matched first
      // and the orderbook will still work correctly but not all orders will be
      // matched ("overpriced" bids will match with "underpriced" asks)
      // TODO: update test logic to determing the expected trade volume at
      // runtime.
      orders.push_front(op1);
    }
    return orders;
  }
};

TEST_F(OrderbookMarketOrderTest, MarketBidOrdersMatchCorrectly) {
  makeMarketBids();
  makeLimitAsks();

  int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
  EXPECT_EQ(ob.GetTradeVolume(), expectedTradedQuantity);
}

TEST_F(OrderbookMarketOrderTest, MarketAskOrdersMatchCorrectly) {
  makeMarketAsks();
  makeLimitBids();

  int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
  EXPECT_EQ(ob.GetTradeVolume(), expectedTradedQuantity);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
