#pragma once

#include "Trade.hpp"
#include <boost/lockfree/queue.hpp>

class TradeQueue {
public:
  static TradeQueue &instance() {
    static TradeQueue instance;
    return instance;
  }

  TradeQueue(const TradeQueue &) = delete;
  TradeQueue &operator=(const TradeQueue &) = delete;

  bool enqueue(const model::Trade &trade);

  bool try_dequeue(model::Trade &trade);

private:
  TradeQueue() = default;
  boost::lockfree::queue<model::Trade> queue_{1024};
};
