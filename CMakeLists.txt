cmake_minimum_required(VERSION 3.16)
project(market_data_feed LANGUAGES CXX)

# Require modern C++ version
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: warnings
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

# Optional: find Boost (networking, Asio, etc.)
find_package(Boost REQUIRED COMPONENTS system)

# Add include directory
include_directories(include)

# Create the main executable
add_executable(market_data_feed
    src/main.cpp
    src/FeedHandler.cpp
)

# Link Boost (or other deps)
target_link_libraries(market_data_feed
    Boost::system
    pthread
)
