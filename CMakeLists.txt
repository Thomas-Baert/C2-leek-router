set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.15)
project(RouterP2P VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(asio REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
        src/network/peer/peer.hpp
        src/network/peer/peer_table.hpp
        src/network/server.hpp
        src/network/handelers/Rebound.hpp)

target_link_libraries(${PROJECT_NAME} PRIVATE asio::asio OpenSSL::SSL)
target_include_directories(${PROJECT_NAME} PUBLIC include)


find_package(tabulate REQUIRED)
find_package(asio REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(RouterP2P PRIVATE 
    tabulate::tabulate 
    asio::asio 
    openssl::openssl
)