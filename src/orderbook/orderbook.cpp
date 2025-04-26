#include "orderbook.hpp"
#include "trade/trade.hpp"
#include <iterator>
#include <memory>
#include <mutex>

int Orderbook::AddOrder(OrderPointer o) {
  std::scoped_lock ordersLock{mutex_};
  auto price = o->GetPrice();
  o->SetId();
  OrderType type_ = o->GetType();
  Side side_ = o->GetSide();
  OrderPointers::iterator order_location;

  // TODO: Stop Order Support
  // TODO: Iceberg Order Support
  // TODO: Pegged Order Support
  // TODO: Trailing Stop Order Support
  // TODO: Fill or Kill Order Support
  switch (type_) {
  case OrderType::Market:
    order_location = HandleMarketOrder(o);
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

void Orderbook::CancelOrder(OrderId orderId) {
  std::scoped_lock ordersLock{mutex_};

  CancelOrderInternal(orderId);
}

void Orderbook::CancelOrderInternal(OrderId orderId) {
  if (orders_.find(orderId) == orders_.end())
    return;

  const auto [order, iterator] = orders_.at(orderId);
  orders_.erase(orderId);

  if (order->GetSide() == Side::Ask) {
    auto price = order->GetPrice();
    auto &orders = ask_orders.at(price);
    orders.erase(iterator);
    if (orders.empty())
      ask_orders.erase(price);
  } else {
    auto price = order->GetPrice();
    auto &orders = bid_orders.at(price);
    orders.erase(iterator);
    if (orders.empty())
      bid_orders.erase(price);
  }
}

// TODO: Complete implemtation.
OrderPointers::iterator Orderbook::HandleMarketOrder(OrderPointer o) {
  OrderPointers::iterator iterator;
  Side side = o->GetSide();
  if (side == Side::Bid) {
    market_bid_orders.push_front(o);
    iterator = market_bid_orders.begin();
  } else {
    market_ask_orders.push_front(o);
    iterator = market_ask_orders.begin();
  }
  return iterator;
}

OrderPointers::iterator Orderbook::HandleLimitOrder(OrderPointer o) {
  OrderPointers::iterator iterator;
  Side side = o->GetSide();
  if (side == Side::Bid) {
    OrderPointers &bids = bid_orders[o->GetPrice()];
    bids.push_back(o);
    iterator = std::prev(bids.end());
  } else {
    OrderPointers &asks = ask_orders[o->GetPrice()];
    asks.push_back(o);
    iterator = std::prev(asks.end());
  }
  return iterator;
}

void Orderbook::MatchOrders() {
  while (true) {
    // Check if there are any orders
    // Process market orders first (execute at any price available)
    while (!market_bid_orders.empty() && !ask_orders.empty()) {
      MatchMarketOrders(market_bid_orders.front(), std::ref(ask_orders));
    }
    while (!market_ask_orders.empty() && !bid_orders.empty()) {
      MatchMarketOrders(market_ask_orders.front(), std::ref(bid_orders));
    }

    // Check if limit orders can be matched
    if (CanMatchLimitOrders())
      MatchLimitOrders(trades);
    else
      break;
  }
}

bool Orderbook::CanMatchLimitOrders() {
  // Get the highest bid and lowest asking price
  //     // For limit orders, check that both orders exist
  if (bid_orders.empty() || ask_orders.empty())
    return false;

  auto &[bidPrice, bids] = *bid_orders.begin();
  auto &[askPrice, asks] = *ask_orders.begin();

  if (bidPrice < askPrice) {
    return false;
  }
  return true;
}

void Orderbook::MatchLimitOrders(Trades &t) {
  auto &[bidPrice, bids] = *bid_orders.begin();
  auto &[askPrice, asks] = *ask_orders.begin();
  while (!bids.empty() && !asks.empty()) {
    auto bid = bids.front();
    auto ask = asks.front();
    Trade trade = Trade::MakeTrade(bid, ask, trade_id_gen.nextId());
    trades.push_back(trade);
    if (bid->IsFilled()) {
      bids.pop_front();
    }
    if (ask->IsFilled()) {
      asks.pop_front();
    }
    // delete price level if list is empty
    if (bids.empty())
      bid_orders.erase(bidPrice);
    if (asks.empty())
      ask_orders.erase(askPrice);
  }
}

void Orderbook::MatchMarketOrders(OrderPointer market_bid_order,
                                  AskOrders &non_market_orders) {
  auto &[askPrice, asks] = *non_market_orders.begin();
  auto ask = asks.front();
  Trade trade =
      Trade::MakeMktTrade(market_bid_order, ask, trade_id_gen.nextId());
  if (ask->IsFilled()) {
    asks.pop_front();
    if (asks.empty()) {
      non_market_orders.erase(askPrice);
    }
  }
  if (market_bid_order->IsFilled()) {
    market_bid_orders.pop_front();
  }
  trades.push_back(trade);
  return;
}

void Orderbook::MatchMarketOrders(OrderPointer market_ask_order,
                                  BidOrders &non_market_orders) {
  auto &[bidPrice, bids] = *non_market_orders.begin();
  auto bid = bids.front();
  Trade trade =
      Trade::MakeMktTrade(bid, market_ask_order, trade_id_gen.nextId());
  if (bid->IsFilled()) {
    bids.pop_front();
    if (bids.empty()) {
      non_market_orders.erase(bidPrice);
    }
  }
  if (market_ask_order->IsFilled()) {
    market_ask_orders.pop_front();
  }
  trades.push_back(trade);
  return;
}

void Orderbook::ShowOrders() {
  std::scoped_lock orderLock{mutex_};
  std::cout << "Bids:" << std::endl;
  std::cout << "(Limit Orders)" << '\n';
  if (bid_orders.empty())
    std::cout << "[NONE]" << std::endl;
  else {
    for (const auto &[price, orders] : bid_orders) {
      std::cout << "Price: " << price << ", Orders: ";
      for (const auto &order : orders) {
        std::cout << "[ID: " << order->GetId()
                  << ", Requested Quantity: " << order->GetQuantity()
                  << ", Remaining Quantity: " << order->GetRemainingQuantity()
                  << "] ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << "(Market Orders)" << '\n';
  if (market_bid_orders.empty())
    std::cout << "[NONE]" << std::endl;
  else {
    for (const auto &order : market_bid_orders) {
      std::cout << "[ID: " << order->GetId()
                << ", Requested Quantity: " << order->GetQuantity()
                << ", Remaining Quantity: " << order->GetRemainingQuantity()
                << "] ";
    }
    std::cout << std::endl;
  }

  std ::cout << "Asks:" << std::endl;
  std::cout << "(Limit Orders)" << '\n';
  if (ask_orders.empty())
    std::cout << "[NONE]" << std::endl;
  else {
    for (const auto &[price, orders] : ask_orders) {
      std::cout << "Price: " << price << ", Orders: ";
      for (const auto &order : orders) {
        std::cout << "[ID: " << order->GetId()
                  << ", Requested Quantity: " << order->GetQuantity()
                  << ", Remaining Quantity: " << order->GetRemainingQuantity()
                  << "] ";
      }
      std::cout << std::endl;
    }
  }
  std::cout << "(Market Orders)" << '\n';
  if (market_ask_orders.empty())
    std::cout << "[NONE]" << std::endl;
  else {
    for (const auto &order : market_ask_orders) {
      std::cout << "[ID: " << order->GetId()
                << ", Requested Quantity: " << order->GetQuantity()
                << ", Remaining Quantity: " << order->GetRemainingQuantity()
                << "] ";
    }
    std::cout << std::endl;
  }
}
