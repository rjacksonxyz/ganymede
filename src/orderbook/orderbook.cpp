#include "orderbook.hpp"
#include <iterator>

int Orderbook::AddOrder(OrderPointer o)
{
  auto price = o->GetPrice();

  // TODO: Market Order Support
  OrderType type_ = o->GetType();
  Side side_ = o->GetSide();
  OrderPointers::iterator order_location;

  // TODO: Stop Order Support
  // TODO: Iceberg Order Support
  // TODO: Pegged Order Support
  // TODO: Trailing Stop Order Support
  // TODO: Fill or Kill Order Support
  switch (type_)
  {
  case OrderType::Market:
    HandleMarketOrder(o);
    break;
  case OrderType::Limit:
    order_location = HandleLimitOrder(o);
    break;
  default:
    throw std::invalid_argument("Order type not supported");
  }
  orders_.insert({o->GetId(), {o, order_location}});
  MatchOrders();
  return 0;
}

void Orderbook::CancelOrder(OrderId orderId)
{
  std::scoped_lock ordersLock{mutex_};

  CancelOrderInternal(orderId);
}

void Orderbook::CancelOrderInternal(OrderId orderId)
{
  if (orders_.find(orderId) == orders_.end())
    return;

  const auto [order, iterator] = orders_.at(orderId);
  orders_.erase(orderId);

  if (order->GetSide() == Side::Ask)
  {
    auto price = order->GetPrice();
    auto &orders = ask_orders.at(price);
    orders.erase(iterator);
    if (orders.empty())
      ask_orders.erase(price);
  }
  else
  {
    auto price = order->GetPrice();
    auto &orders = bid_orders.at(price);
    orders.erase(iterator);
    if (orders.empty())
      bid_orders.erase(price);
  }
}

// TODO: Complete implemtation.
int Orderbook::HandleMarketOrder(OrderPointer o)
{
  OrderPointers::iterator iterator;
  Side side = o->GetSide();
  if (side == Side::Bid)
    market_buy_orders.push(o);
  else
    market_sell_orders.push(o);
  return 0;
}

OrderPointers::iterator Orderbook::HandleLimitOrder(OrderPointer o)
{
  OrderPointers::iterator iterator;
  Side side = o->GetSide();
  if (side == Side::Bid)
  {
    OrderPointers &bids = bid_orders[o->GetPrice()];
    bids.push_back(o);
    iterator = std::prev(bids.end());
  }
  else
  {
    OrderPointers &asks = ask_orders[o->GetPrice()];
    asks.push_back(o);
    iterator = std::prev(asks.end());
  }
  return iterator;
}

// TODO: Fix the bug where this runs indefinitely
void Orderbook::MatchOrders()
{
  // retrieve the highest bids and lowest asks
  std::vector<Trade> trades;
  while (true)
  {
    // sleep for 500 milliseconds
    if (bid_orders.empty() || ask_orders.empty())
      break;

    auto &[bidPrice, bids] = *bid_orders.begin();
    auto &[askPrice, asks] = *ask_orders.begin();
    if (bidPrice < askPrice)
    {
      break;
    }
    while (!bids.empty() && !asks.empty())
    {
      auto bid = bids.front();
      auto ask = asks.front();
      Trade trade = MakeTrade(bid, ask);
      std::cout << trade << std::endl;
      trades.push_back(trade);
      if (bid->IsFilled())
      {
        bids.pop_front();
      }
      if (ask->IsFilled())
      {
        asks.pop_front();
      }
      // delete price level if list is empty
      if (bids.empty())
        bid_orders.erase(bidPrice);
      if (asks.empty())
        ask_orders.erase(askPrice);
    }
  }
}

void Orderbook::ShowOrders()
{
  std::cout << "Bids:" << std::endl;
  if (bid_orders.empty())
    std::cout << "[NONE]" << std::endl;
  else
  {
    for (const auto &[price, orders] : bid_orders)
    {
      std::cout << "Price: " << price << ", Orders: ";
      for (const auto &order : orders)
      {
        std::cout << "[ID: " << order->GetId() << ", Quantity: " << order->GetRemainingQuantity() << "] ";
      }
      std::cout << std::endl;
    }
  }

  std::cout << "Asks:" << std::endl;
  if (ask_orders.empty())
    std::cout << "[NONE]" << std::endl;
  else
  {
    for (const auto &[price, orders] : ask_orders)
    {
      std::cout << "Price: " << price << ", Orders: ";
      for (const auto &order : orders)
      {
        std::cout << "[ID: " << order->GetId() << ", Quantity: " << order->GetRemainingQuantity() << "] ";
      }
      std::cout << std::endl;
    }
  }
}

// TODO: Complete implementation
Trade Orderbook::MakeTrade(OrderPointer bid, OrderPointer ask)
{
  Quantity q = std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());
  Trade t = Trade(bid, ask, q, bid->GetPrice(), trade_id_gen.nextId());
  bid->Fill(q);
  ask->Fill(q);
  return t;
}
