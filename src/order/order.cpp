#include "order.hpp"

std::atomic<uint64_t> OrderIdGenerator::counter{0};

void Order::Order::Fill(Quantity q)
{
    remaining_quantity -= q > quantity ? quantity : q;
}