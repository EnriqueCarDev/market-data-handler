#include "MessageParser.hpp"
#include "WebSocketClient.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <memory>
#include <thread>

int main() {

  const std::string host = "stream.binance.com";
  const std::string port = "9443";
  const std::string target = "/ws/btcusdt@trade";

  net::io_context ioc;
  ssl::context ctx{ssl::context::tlsv12_client};

  ctx.set_verify_mode(ssl::verify_none);

  auto session = std::make_shared<WebSocketSession>(ioc, ctx);

  session->run(host.c_str(), port.c_str(), target.c_str());

  std::thread consumer_thread([session] {
    MessageParser message_parser(session->get_queue());
    while (true) {
      std::cout << "inside loop" << std::endl;
      message_parser.run();
    }
  });

  ioc.run();
  consumer_thread.join();

  return EXIT_SUCCESS;
}