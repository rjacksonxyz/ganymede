#include "../src/orderbook/orderbook.hpp"
#include <benchmark/benchmark.h>

void makeBids(Orderbook &ob);
void makeAsks(Orderbook &ob);
static void limitOrderTest(benchmark::State &s);

static void limitOrderTest(benchmark::State &s) {
  for (auto _ : s) {
    Orderbook ob = Orderbook();
    std::thread bid_thread(makeBids, std::ref(ob));
    std::thread ask_thread(makeAsks, std::ref(ob));
    bid_thread.join();
    ask_thread.join();
  }
}

void makeAsks(Orderbook &ob) {
  for (int i = 0; i < 10; ++i) {
    Quantity q = 35 + i;
    Price p = 59.50 + i * 0.10;
    auto op1 = std::make_shared<Order>(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op1);
    auto op2 = std::make_shared<Order>(Side::Ask, q, OrderType::Limit, p);
    ob.AddOrder(op2);
  }
}

void makeBids(Orderbook &ob) {
  for (int i = 0; i < 10; ++i) {
    Quantity q = 70 + i;
    Price p = 59.90 - i * 0.10;
    auto op1 = std::make_shared<Order>(Side::Bid, q, OrderType::Limit, p);
    ob.AddOrder(op1);
    auto op2 = std::make_shared<Order>(Side::Bid, q, OrderType::Limit, p);
    ob.AddOrder(op2);
  }
}

BENCHMARK(limitOrderTest);

BENCHMARK_MAIN();
