#include "../src/orderbook/orderbook.hpp"
#include <benchmark/benchmark.h>

void makeBids(Orderbook &ob);
void makeAsks(Orderbook &ob);
static void limitOrderSequentialTest(benchmark::State &s);

static void limitOrderSequentialTest(benchmark::State &s)
{
    for (auto _ : s)
    {
        Orderbook ob = Orderbook();
        makeBids(ob);
        makeAsks(ob);
    }
}

void makeAsks(Orderbook &ob)
{
    for (int i = 0; i < 10; ++i)
    {
        Quantity q = 35 + i;
        Price p = 59.50 + i * 0.10;
        Order op1(Side::Ask, q, OrderType::Limit, p);
        ob.AddOrder(op1);
        Order op2(Side::Ask, q, OrderType::Limit, p);
        ob.AddOrder(op2);
    }
}

void makeBids(Orderbook &ob)
{
    for (int i = 0; i < 10; ++i)
    {
        Quantity q = 70 + i;
        Price p = 59.90 - i * 0.10;
        Order op1(Side::Bid, q, OrderType::Limit, p);
        ob.AddOrder(op1);
        Order op2(Side::Bid, q, OrderType::Limit, p);
        ob.AddOrder(op2);
    }
}

BENCHMARK(limitOrderSequentialTest);

BENCHMARK_MAIN();
