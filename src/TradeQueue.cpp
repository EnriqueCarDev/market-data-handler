#include "TradeQueue.hpp"

bool TradeQueue::enqueue(const model::Trade &trade) {
  return queue_.push(trade);
}

bool TradeQueue::try_dequeue(model::Trade &trade) { return queue_.pop(trade); }