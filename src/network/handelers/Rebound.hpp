#pragma once

#include <iostream>
#include "abstract_handeler.hpp"
#include <string>

using asio::ip::tcp;

class Rebound: public AbstractHandeler {
    unsigned short port;
    std::string ip = "127.0.0.1";

public:
    explicit Rebound(const unsigned short port): port(port) {}
    Rebound(const unsigned short port, std::string ip) : port(port), ip(std::move(ip)) {}

    asio::awaitable<void> handle_client(tcp::socket socket) override {
        try {
            char data[1024];
            auto executor = co_await asio::this_coro::executor;
            tcp::socket destination_socket(executor);
            tcp::resolver resolver(executor);

            auto endpoints = co_await resolver.async_resolve(ip, std::to_string(this->port), asio::use_awaitable);
            co_await asio::async_connect(destination_socket, endpoints, asio::use_awaitable);

            for (;;) {
                std::size_t n = co_await socket.async_read_some(asio::buffer(data), asio::use_awaitable);

                co_await asio::async_write(destination_socket, asio::buffer(data, n), asio::use_awaitable);
            }
        } catch (std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
};