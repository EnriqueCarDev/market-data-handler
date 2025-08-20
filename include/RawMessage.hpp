#pragma once

#include <string>

enum Exchange { BINANCE, KRAKEN, COINBASE };

struct RawMessage {
  Exchange exchange;
  std::string payload;
};