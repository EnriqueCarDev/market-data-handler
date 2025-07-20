#include "WebSocketClient.hpp"
#include <iostream>

void fail(beast::error_code ec, char const *what) {
  std::cerr << what << ": " << std::endl;
}

WebSocketSession::WebSocketSession(net::io_context &ioc, ssl::context &ctx)
    : resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc), ctx) {}

void WebSocketSession::run(char const *host, char const *port,
                           char const *text) {
  if (!SSL_set_tlsext_host_name(ws_.next_layer().native_handle(), host)) {
    beast::error_code ec{static_cast<int>(::ERR_get_error()),
                         net::error::get_ssl_category()};
    std::cerr << ec.message() << std::endl;
    return;
  }

  ws_.next_layer().set_verify_callback(ssl::host_name_verification(host));

  host_ = host;
  text_ = text;

  resolver_.async_resolve(
      host, port,
      beast::bind_front_handler(&WebSocketSession::on_resolve,
                                shared_from_this()));
}

void WebSocketSession::on_resolve(beast::error_code ec,
                                  tcp::resolver::results_type results) {
  if (ec) {
    return fail(ec, "resolve");
  }

  beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));

  beast::get_lowest_layer(ws_).async_connect(
      results, beast::bind_front_handler(&WebSocketSession::on_connect,
                                         shared_from_this()));
}

void WebSocketSession::on_connect(
    beast::error_code ec, tcp::resolver::results_type::endpoint_type ep) {
  if (ec)
    return fail(ec, "connect");

  beast::get_lowest_layer(ws_).expires_after(std::chrono::seconds(30));
  host_ += ':' + std::to_string(ep.port());

  // SSL handshake
  ws_.next_layer().async_handshake(
      ssl::stream_base::client,
      beast::bind_front_handler(&WebSocketSession::on_ssl_handshake,
                                shared_from_this()));
}

void WebSocketSession::on_ssl_handshake(beast::error_code ec) {
  if (ec)
    return fail(ec, "ssl_handshake");

  beast::get_lowest_layer(ws_).expires_never();

  ws_.set_option(
      websocket::stream_base::decorator([](websocket::request_type &req) {
        req.set(http::field::user_agent,
                std::string(BOOST_BEAST_VERSION_STRING) +
                    " websocket-client-async-ssl");
      }));

  ws_.async_handshake(host_, "/",
                      beast::bind_front_handler(&WebSocketSession::on_handshake,
                                                shared_from_this()));
}

void WebSocketSession::on_handshake(beast::error_code ec) {
  if (ec)
    return fail(ec, "handshake");

  ws_.async_write(net::buffer(text_),
                  beast::bind_front_handler(&WebSocketSession::on_write,
                                            shared_from_this()));
}

void WebSocketSession::on_write(beast::error_code ec,
                                std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec)
    return fail(ec, "write");

  ws_.async_read(buffer_, beast::bind_front_handler(&WebSocketSession::on_read,
                                                    shared_from_this()));
}

void WebSocketSession::on_read(beast::error_code ec,
                               std::size_t bytes_transferred) {
  boost::ignore_unused(bytes_transferred);

  if (ec)
    return fail(ec, "read");

  ws_.async_close(websocket::close_code::normal,
                  beast::bind_front_handler(&WebSocketSession::on_close,
                                            shared_from_this()));
}

void WebSocketSession::on_close(beast::error_code ec) {
  if (ec)
    return fail(ec, "close");

  std::cout << beast::make_printable(buffer_.data()) << std::endl;
}