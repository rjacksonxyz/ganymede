#include "orderbook.hpp"

int OrderBook::AddOrder(Order::OrderPointer o) {
  auto price = o->GetPrice();

  // TODO: Market Order Support
  if (o->GetType() == Order::OrderType::Market) {
  }
  // TODO: Limit Order Support
  // TODO: Stop Order Support
  // TODO: Iceberg Order Support
  // TODO: Pegged Order Support
  // TODO: Trailing Stop Order Support
  // TODO: Fill or Kill Order Support
  if (o->GetSide() == Order::Side::Bid) {
    bid_orders[price].push_back(o);
  } else {
    ask_orders[price].push_back(o);
  }
  return 0;
}

// TODO: Complete implemtation.
int OrderBook::HandleMarketOrder(Order::OrderPointer o) {
  Order::Side side = o->GetSide();
  if (side == Order::Side::Bid) {
    if (!ask_orders.empty())
      const auto&[lowest_ask, id] = *(ask_orders.rbegin());
    else 
      market_buy_orders.push(o);
  }
  else {
    if (!bid_orders.empty())
      const auto&[highest_bid, _] = *(bid_orders.begin());
    else
      market_sell_orders.push(o);
  }

}
// TODO: Complete implementation
Trade OrderBook::MakeTrade(Order::OrderPointer o1, Order::OrderPointer o2) {
  return;
}
