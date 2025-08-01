#include "ThreadSafeQueue.hpp"
#include "Trade.hpp"

class MessageParser {
private:
  ThreadSafeQueue<Trade> trade_queue_;
  ThreadSafeQueue<std::string> raw_queue_;

  Trade parse_trade(const std::string &message) {}

public:
  MessageParser(ThreadSafeQueue<std::string> &raw_queue)
      : raw_queue_{raw_queue} {};
  void run() {};
};
