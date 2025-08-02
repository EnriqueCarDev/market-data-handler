#include "MessageParser.hpp"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

MessageParser::MessageParser(
    std::shared_ptr<ThreadSafeQueue<std::string>> raw_queue,
    std::shared_ptr<ThreadSafeQueue<Trade>> trade_queue)
    : raw_queue_{std::move(raw_queue)}, trade_queue_{std::move(trade_queue)} {};

Trade MessageParser::parse_trade(const std::string &message) {
  json j = json::parse(message);

  return Trade{j["s"].get<std::string>(),
               j["t"].get<uint64_t>(),
               std::stod(j["p"].get<std::string>()),
               std::stod(j["q"].get<std::string>()),
               j["T"].get<uint64_t>(),
               j["m"].get<bool>()};
}

void MessageParser::run() {
  std::string raw_message;
  raw_queue_->wait_and_pop(raw_message);
  std::cout << "message received: " << raw_message << std::endl;
  const Trade trade = parse_trade(raw_message);
  trade_queue_->push(trade);
}
