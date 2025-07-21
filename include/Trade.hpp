#pragma once

#include <cstdint>
#include <string>

namespace model {
struct Trade {
  char symbol[12];
  uint64_t trade_id;
  double price;
  double quantity;
  uint64_t timestamp;
  bool is_maker;
};
}