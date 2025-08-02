#pragma once

#include <cstdint>
#include <string>

struct Trade {
  std::string symbol;
  uint64_t trade_id;
  double price;
  double quantity;
  uint64_t timestamp;
  bool is_maker;
};
