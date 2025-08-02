#pragma once

#include "ThreadSafeQueue.hpp"
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

void fail(beast::error_code ec, char const *what);

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
  WebSocketSession(net::io_context &ioc, ssl::context &ctx,
                   std::shared_ptr<ThreadSafeQueue<std::string>> raw_queue);
  void run(char const *host, char const *port, char const *target);
  void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
  void on_connect(beast::error_code ec,
                  tcp::resolver::results_type::endpoint_type ep);
  void on_ssl_handshake(beast::error_code ec);
  void on_handshake(beast::error_code ec);
  void on_read(beast::error_code ec, std::size_t bytes_transferred);
  void do_read();
  void on_close(beast::error_code ec);

private:
  tcp::resolver resolver_;
  websocket::stream<ssl::stream<beast::tcp_stream>> ws_;
  beast::flat_buffer buffer_;
  std::shared_ptr<ThreadSafeQueue<std::string>> queue_;
  std::string host_;
  std::string target_;
};
