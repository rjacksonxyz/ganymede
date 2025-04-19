#include "orderbook.hpp"
#include <iostream>

int Orderbook::AddOrder(Order::OrderPointer o) {
  auto price = o->GetPrice();

  // TODO: Market Order Support
  Order::OrderType type_ = o->GetType();
  Order::Side side_ = o->GetSide();

  switch (type_) {
    case Order::OrderType::Market:
      HandleMarketOrder(o);
      break;
    case Order::OrderType::Limit:
      HandleLimitOrder(o);
      break;
    default:
      // TODO: Stop Order Support
      // TODO: Iceberg Order Support
      // TODO: Pegged Order Support
      // TODO: Trailing Stop Order Support
      // TODO: Fill or Kill Order Support
      std::cout << (int)type_ << " order type not supported" << '\n';
  }
  MatchOrders();
  return 0;
}

// TODO: Complete implemtation.
int Orderbook::HandleMarketOrder(Order::OrderPointer o) {
  Order::Side side = o->GetSide();
  if (side == Order::Side::Bid)
    market_buy_orders.push(o);
  else 
    market_sell_orders.push(o);
  return 0;
}

int Orderbook::HandleLimitOrder(Order::OrderPointer o) {
  Order::Side side = o->GetSide();
  if (side == Order::Side::Bid)
    bid_orders[o->GetPrice()].push_back(o);
  else 
    ask_orders[o->GetPrice()].push_back(o);
  return 0;
}


void Orderbook::MatchOrders() {
  // retrieve the highest bids and lowest asks
  std::vector<Trade> trades;
  while (true) {
    if (bid_orders.empty() || ask_orders.empty())
      break;
    
    auto& [bidPrice, bids] = *bid_orders.begin();
    auto& [askPrice, asks] = *ask_orders.begin();
    if (bidPrice < askPrice)
      break;
    while (!bids.empty() && !asks.empty()){
      auto bid = bids.front();
      auto ask = asks.front();
      auto trade = MakeTrade(bid, ask);
      trades.push_back(trade);
      if (bid->IsFilled()) {
        bids.pop_front();
      }
      if (ask->IsFilled()) {
        asks.pop_front();
      }
    }
  }
}

// TODO: Complete implementation
Trade Orderbook::MakeTrade(Order::OrderPointer bid, Order::OrderPointer ask) {
  Order::Quantity q = std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());
  Trade t = Trade(bid, ask, bid->GetType(), trade_id_gen.nextId());
  bid->Fill(q);
  ask->Fill(q);
}
