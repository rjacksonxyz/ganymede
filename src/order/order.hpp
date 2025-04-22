#pragma once
#include <chrono>
#ifndef GYMD_ORDER
#define GYMD_ORDER

#include <atomic>
#include <cstdint>
#include <iostream>
#include <list>
#include <memory>

using OrderId = uint64_t;
using Price = double;
using Quantity = uint64_t;
enum class Side
{
  Bid,
  Ask,
};

class OrderIdGenerator
{
private:
  static inline std::atomic<uint64_t> counter{0};

public:
  static uint64_t generateId()
  {
    return counter.fetch_add(1, std::memory_order_relaxed);
  }
};

enum class OrderType
{
  Market,
  Limit,
  // GoodForDay,
  // GoodTillCancel,
  // ImmediateOrCancel,
  // FillOrKill,
  // StopLoss,
  // SellStop,
  // BuyStop,
  // StopLimit,
  // TrailingStop,
  // PegBest,
  // MidPricePeg,
  // AtTheOpen,
  // Iceberg,
};

class Order
{
private:
  OrderId id;
  Side side;
  uint64_t quantity;
  uint64_t remaining_quantity;
  OrderType type_;
  Price price;
  int64_t timestamp;

public:
  /*
   Constructor
   @param s: Side of the order
   @param q: Quantity of the order
   @param t: Type of the order
   @param p: Price of the order
  */
  Order(Side s, Quantity q, OrderType t, Price p)
      : side(s), quantity(q), type_(t), price(p),
        timestamp(std::chrono::system_clock::now().time_since_epoch().count())
  {
    remaining_quantity = quantity;
  }
  ~Order() = default;
  Order(const Order &) = default;
  Order(Order &&) = default;
  Order &operator=(const Order &) = default;
  Order &operator=(Order &&) = default;

  OrderId GetId() const { return id; }
  void SetId() { id = OrderIdGenerator::generateId(); }
  Side GetSide() const { return side; }
  uint64_t GetQuantity() const { return quantity; }
  uint64_t GetRemainingQuantity() const { return remaining_quantity; }
  OrderType GetType() const { return type_; }
  int64_t GetTimestamp() const { return timestamp; }
  Price GetPrice() const { return price; }
  void SetPrice(Price p) { price = p; }
  /*
   * Reduces remaining_quantity by Quantity q
   * @param q Quantity of instrument
   */
  void Fill(Quantity q);
  bool IsFilled() const { return GetRemainingQuantity() == 0; }
};
using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;
#endif
