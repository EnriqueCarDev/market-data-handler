#pragma once

#include "ThreadSafeQueue.hpp"
#include "Trade.hpp"

class MessageParser {
private:
  std::shared_ptr<ThreadSafeQueue<std::string>> raw_queue_;
  std::shared_ptr<ThreadSafeQueue<Trade>> trade_queue_;

  Trade parse_trade(const std::string &message);

public:
  MessageParser(std::shared_ptr<ThreadSafeQueue<std::string>> raw_queue,
                std::shared_ptr<ThreadSafeQueue<Trade>> trade_queue);
  void run();
};
