#include "orderbook.hpp"
#include "order/order.hpp"
#include <cstddef>
#include <iterator>
#include <memory>
#include <mutex>
#include <queue>

int Orderbook::AddOrder(OrderPointer o) {
  std::scoped_lock ordersLock{mutex_};
  auto price = o->GetPrice();
  o->SetId();

  // TODO: Market Order Support
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
  std::vector<Trade> trades;
  while (true) {
    // check if there are any orders
    if (bid_orders.empty() && ask_orders.empty() && market_bid_orders.empty() &&
        market_ask_orders.empty())
      break;

    // Process market orders first (execute at any price available)
    if (!market_bid_orders.empty() || !market_ask_orders.empty()) {
      MatchMarketOrders(trades);
      break;
    }

    // For limit orders, check that both orders exist
    if (bid_orders.empty() || ask_orders.empty())
      break;

    // Get the highest bid and lowest ask
    auto &[bidPrice, bids] = *bid_orders.begin();
    auto &[askPrice, asks] = *ask_orders.begin();

    if (bidPrice < askPrice) {
      break;
    }

    // process limit orders
    while (!bids.empty() && !asks.empty()) {
      auto bid = bids.front();
      auto ask = asks.front();
      Trade trade = Trade::MakeTrade(bid, ask, trade_id_gen.nextId());
      std::cout << trade << std::endl;
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
}

void Orderbook::MatchMarketOrders(Orderbook::Trades &t) {
  auto market_bid =
      !market_bid_orders.empty() ? market_bid_orders.front() : nullptr;
  auto market_ask =
      !market_ask_orders.empty() ? market_ask_orders.front() : nullptr;
  if (market_bid != nullptr && !ask_orders.empty()) {
    // Use structured binding for asks with proper scoping
    auto &[askPrice, asks] = *ask_orders.begin();
    auto ask = asks.front();
    Trade trade = Trade::MakeMktTrade(market_bid, ask, trade_id_gen.nextId());
    if (ask->IsFilled()) {
      asks.pop_front();
      if (asks.empty()) {
        ask_orders.erase(askPrice);
      }
    }
    if (market_bid->IsFilled()) {
      market_bid_orders.pop_front();
    }
    t.push_back(trade);
    std::cout << trade << std::endl;
  }

  if (market_ask != nullptr && !bid_orders.empty()) {
    // Use structured binding for bids with proper scoping
    auto &[bidPrice, bids] = *bid_orders.begin();
    std::cout << "processing market ask" << '\n';
    auto bid = bids.front();
    Trade trade = Trade::MakeMktTrade(bid, market_ask, trade_id_gen.nextId());
    if (bid->IsFilled()) {
      bids.pop_front();
      if (bids.empty()) {
        bid_orders.erase(bidPrice);
      }
    }
    if (market_ask->IsFilled()) {
      market_ask_orders.pop_front();
    }
    t.push_back(trade);
    std::cout << trade << std::endl;
  }
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
