#include "../src/orderbook/orderbook.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <algorithm>
#include <memory>

class OrderbookMarketOrderTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        askQuantity = 0;
        bidQuantity = 0;
    }

    static const int ORDER_N = 20;
    int askQuantity = 0;
    int bidQuantity = 0;
    Orderbook ob;

public:
    void makeMarketBids()
    {
        for (int i = 0; i < ORDER_N; ++i)
        {
            Quantity q = 70 + i;
            auto op1 = std::make_shared<Order>(Side::Bid, q, OrderType::Market, 0);
            ob.AddOrder(op1);
            bidQuantity += q;
        }
    }

    void makeLimitBids()
    {
        for (int i = 0; i < ORDER_N; ++i)
        {
            Quantity q = 70 + i;
            Price p = 59.50 + i * 0.10;
            auto op1 = std::make_shared<Order>(Side::Bid, q, OrderType::Limit, p);
            ob.AddOrder(op1);
            bidQuantity += q;
        }
    }

    void makeMarketAsks()
    {
        for (int i = 0; i < ORDER_N / 2; ++i)
        {
            Quantity q = 35 + i;
            auto op1 = std::make_shared<Order>(Side::Ask, q, OrderType::Market, 0);
            ob.AddOrder(op1);
            auto op2 = std::make_shared<Order>(Side::Ask, q, OrderType::Market, 0);
            ob.AddOrder(op2);
            askQuantity += q * 2;
        }
    }

    void makeLimitAsks()
    {
        for (int i = 0; i < ORDER_N / 2; ++i)
        {
            Quantity q = 35 + i;
            Price p = 59.50 + i * 0.10;
            auto op1 = std::make_shared<Order>(Side::Ask, q, OrderType::Limit, p);
            ob.AddOrder(op1);
            auto op2 = std::make_shared<Order>(Side::Ask, q, OrderType::Limit, p);
            ob.AddOrder(op2);
            askQuantity += q * 2;
        }
    }
};

TEST_F(OrderbookMarketOrderTest, MarketBidOrdersMatchCorrectly)
{
    std::thread bid_thread(&OrderbookMarketOrderTest::makeMarketBids, this);
    std::thread ask_thread(&OrderbookMarketOrderTest::makeLimitAsks, this);

    ask_thread.join();
    bid_thread.join();

    int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
    EXPECT_EQ(ob.GetTradeVolume(), expectedTradedQuantity);
}

TEST_F(OrderbookMarketOrderTest, MarketAskOrdersMatchCorrectly)
{
    std::thread ask_thread(&OrderbookMarketOrderTest::makeMarketAsks, this);
    std::thread bid_thread(&OrderbookMarketOrderTest::makeLimitBids, this);

    ask_thread.join();
    bid_thread.join();

    int expectedTradedQuantity = std::min(askQuantity, bidQuantity);
    EXPECT_EQ(ob.GetTradeVolume(), expectedTradedQuantity);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
