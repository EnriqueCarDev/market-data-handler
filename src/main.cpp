#include "WebSocketClient.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>

int main() {

  const std::string host = "stream.binance.com";
  const std::string port = "9443";
  const std::string target = "/ws/btcusdt@trade";

  net::io_context ioc;
  ssl::context ctx{ssl::context::tlsv12_client};

  ctx.set_verify_mode(ssl::verify_none);

  std::make_shared<WebSocketSession>(ioc, ctx)->run(host.c_str(), port.c_str(),
                                                    target.c_str());

  ioc.run();

  return EXIT_SUCCESS;
}