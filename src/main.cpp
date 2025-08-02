#include "MessageParser.hpp"
#include "WebSocketClient.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/thread_pool.hpp>
#include <memory>
#include <thread>

int main() {

  const std::string host = "stream.binance.com";
  const std::string port = "9443";
  const std::string target = "/ws/btcusdt@trade";

  net::io_context ioc;
  ssl::context ctx{ssl::context::tlsv12_client};

  ctx.set_verify_mode(ssl::verify_none);

  auto raw_queue{std::make_shared<ThreadSafeQueue<std::string>>()};
  auto trade_queue{std::make_shared<ThreadSafeQueue<Trade>>()};

  auto session = std::make_shared<WebSocketSession>(ioc, ctx, raw_queue);

  session->run(host.c_str(), port.c_str(), target.c_str());
  boost::asio::thread_pool parse_pool(2);

  boost::asio::post(parse_pool, [raw_queue, trade_queue] {
    MessageParser message_parser(raw_queue, trade_queue);
    while (true) {
      message_parser.run();
    }
  });

  ioc.run();
  parse_pool.join();

  return EXIT_SUCCESS;
}