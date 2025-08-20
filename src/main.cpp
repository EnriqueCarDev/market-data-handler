#include "MessageParser.hpp"
#include "ThreadPool.hpp"
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

  auto raw_queue{std::make_shared<ThreadSafeQueue<RawMessage>>()};
  auto trade_queue{std::make_shared<ThreadSafeQueue<Trade>>()};

  auto binance_session = std::make_shared<WebSocketSession>(ioc, ctx, raw_queue,
                                                            Exchange::BINANCE);

  binance_session->run(host.c_str(), port.c_str(), target.c_str());

  ThreadPool pool;
  pool.function_input([raw_queue, trade_queue] {
    MessageParser message_parser(raw_queue, trade_queue);
    while (true) {
      message_parser.run();
    }
  });

  ioc.run();

  return EXIT_SUCCESS;
}