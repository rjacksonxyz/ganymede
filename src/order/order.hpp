#ifndef GYMD_ORDER
#define GYMD_ORDER

#include <atomic>
#include <cstdint>
#include <list>
#include <memory>

namespace Order {
using OrderId = uint64_t;
using Price = double;
using Quantity = uint64_t;
enum class Side {
  Bid,
  Ask,
};

class OrderIdGenerator {
private:
  static std::atomic<uint64_t> counter;

public:
  static uint64_t generateId() {
    return counter.fetch_add(1, std::memory_order_relaxed);
  }
};

enum class OrderType {
  Market,
  Limit,
  GoodForDay,
  GoodTillCancel,
  ImmediateOrCancel,
  FillOrKill,
  StopLoss,
  SellStop,
  BuyStop,
  StopLimit,
  TrailingStop,
  PegBest,
  MidPricePeg,
  AtTheOpen,
  Iceberg,
};

class Order {
private:
  OrderId id;
  Side side;
  uint64_t quantity;
  uint64_t remaining_quantity;
  OrderType type_;
  Price price;

public:
  Order(Side s, Quantity q, OrderType t, Price p)
      : side(s), quantity(q), type_(t), price(p) {
    id = OrderIdGenerator::generateId();
    remaining_quantity = quantity;
  }
  OrderId GetId() const { return id; }
  Side GetSide() const { return side; }
  uint64_t GetQuantity() const { return quantity; }
  uint64_t GetRemainingQuantity() const { return remaining_quantity; }
  OrderType GetType() const { return type_; }
  Price GetPrice() const { return price; }
};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

} // namespace Order
#endif
