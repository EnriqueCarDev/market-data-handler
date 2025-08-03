#pragma once

#include <string_view>

enum Exchange { BINANCE, KRAKEN, COINBASE };

struct RawMessage {
  Exchange exchange;
  std::string_view payload;
};