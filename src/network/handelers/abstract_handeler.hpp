#pragma once
#include <asio.hpp>
#include <asio/awaitable.hpp>

class AbstractHandeler {
public:
    virtual ~AbstractHandeler() = default;
    virtual asio::awaitable<void> handle_client(asio::ip::tcp::socket socket) = 0;
};
