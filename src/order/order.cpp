#include "order.hpp"
#include <iostream>
void Order::Order::Fill(Quantity q)
{
    remaining_quantity -= q > quantity ? quantity : q;
}