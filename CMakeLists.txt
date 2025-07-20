cmake_minimum_required(VERSION 3.16)
project(market_data_feed LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Find Boost with required components
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)

link_directories(${OPENSSL_LIBRARIES})

# Main executable
add_executable(market_data_feed
    src/main.cpp
    src/WebSocketClient.cpp
    
)

# Include directories
target_include_directories(market_data_feed PRIVATE
    include
    ${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR}
)

# Link Boost and pthread
target_link_libraries(market_data_feed PRIVATE
    Boost::system
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
    pthread
)
