#ifndef GYMD_ORDER
#define GYMD_ORDER

#include <vector>

namespace Order {
using OrderId = unsigned long;
using Price = double;
enum class Side {
  Buy,
  Sell,
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
  unsigned long quantity;
  OrderType type;
  Price price;

public:
};

using OrderPointer = Order *;
using OrdersPointer = std::vector<OrderPointer>;
} // namespace Order
#endif
