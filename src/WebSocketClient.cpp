#include "WebSocketClient.hpp"

void fail(beast::error_code ec, char const *what) {
  std::cerr << what << ": " << std::endl;
}
