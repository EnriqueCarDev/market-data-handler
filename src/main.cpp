#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
using tcp = net::ip::tcp;

int main() {
  try {
    // IO Context and SSL Context
    net::io_context ioc;
    ssl::context ctx{ssl::context::tlsv12_client};

    // Verify SSL certs
    ctx.set_default_verify_paths();

    // WebSocket stream with SSL
    websocket::stream<beast::ssl_stream<tcp::socket>> ws{ioc, ctx};

    // Resolve the Binance WSS endpoint
    const std::string host = "stream.binance.com";
    const std::string port = "9443";
    const std::string target = "/ws/btcusdt@trade";

    tcp::resolver resolver{ioc};
    auto const results = resolver.resolve(host, port);

    // Connect TCP
    net::connect(ws.next_layer().next_layer(), results.begin(), results.end());

    // SSL Handshake
    ws.next_layer().handshake(ssl::stream_base::client);

    // WebSocket handshake
    ws.handshake(host, target);

    std::cout << "✅ Connected to Binance WebSocket\n";

    // Receive messages
    for (;;) {
      beast::flat_buffer buffer;
      ws.read(buffer);
      std::cout << beast::make_printable(buffer.data()) << "\n";
    }

  } catch (const std::exception &e) {
    std::cerr << "❌ Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
