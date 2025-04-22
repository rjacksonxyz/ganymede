#include "order.hpp"
#include <iostream>
void Order::Fill(Quantity q)
{
    if (q > remaining_quantity)
    {
        remaining_quantity = 0;
    }
    else
    {
        remaining_quantity -= q;
    }
}