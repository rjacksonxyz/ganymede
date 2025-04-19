#include "order.hpp"
#include <iostream>
void Order::Order::Fill(Quantity q)
{
    remaining_quantity -= q > quantity ? quantity : q;
    std::string side_ = side == Side::Bid ? "bid" : "ask";
    std::cout << side_ << " filled " << q << " of " << quantity << " at " << price << std::endl;
}