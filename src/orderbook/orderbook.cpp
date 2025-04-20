#include "orderbook.hpp"
#include <cstddef>
#include <iterator>
#include <mutex>

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
int Orderbook::HandleMarketOrder(OrderPointer o) {
  OrderPointers::iterator iterator;
  Side side = o->GetSide();
  if (side == Side::Bid)
    market_bid_orders.push(o);
  else
    market_ask_orders.push(o);
  return 0;
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
  // retrieve the highest bids and lowest asks
  std::vector<Trade> trades;
  while (true) {
    // check if there are any orders
    if (bid_orders.empty() || ask_orders.empty())
      break;

    auto &[bidPrice, bids] = *bid_orders.begin();
    auto &[askPrice, asks] = *ask_orders.begin();
    // process market orders first (execute at any price avaialbe)
    while (!market_bid_orders.empty() || !market_ask_orders.empty()) {
      auto market_bid =
          !market_bid_orders.empty() ? market_bid_orders.front() : NULL;
      auto market_ask =
          !market_ask_orders.empty() ? market_ask_orders.front() : NULL;
      Trade trade;
      if (market_bid != NULL) {
        // match market bid
        auto ask = asks.front();
        trade = Trade::MakeTrade(market_bid, ask, trade_id_gen.nextId());
        asks.pop_front();
        if (ask->IsFilled()) {
          asks.pop_front();
        }
      }
      if (market_ask != NULL) {
        auto bid = bids.front();
        trade = Trade::MakeTrade(bid, market_ask, trade_id_gen.nextId());
        if (bid->IsFilled()) {
          bids.pop_front();
        }
      }
      trades.push_back(trade);
      continue;
    }
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

void Orderbook::ShowOrders() {
  std::scoped_lock orderLock{mutex_};
  std::cout << "Bids:" << std::endl;
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

  std::cout << "Asks:" << std::endl;
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
}
