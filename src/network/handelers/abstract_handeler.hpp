#include <asio.hpp>
#include <asio/awaitable.hpp>

#ifndef ABSTRACT_HANDELER_HPP
#define ABSTRACT_HANDELER_HPP

class AbstractHandeler {
    std::unique_ptr<AbstractHandeler> previous;
public:
    virtual ~AbstractHandeler() = default;
    virtual asio::awaitable<void> handle_client(asio::ip::tcp::socket socket) = 0;
};

#endif