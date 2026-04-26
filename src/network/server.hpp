#pragma once

#include "handelers/abstract_handeler.hpp"
#include <memory>
#include <asio.hpp>
#include <iostream>

using namespace std;
using asio::ip::tcp;

class Server {
    vector<shared_ptr<AbstractHandeler>> handlers;
    asio::io_context io_context;
    tcp::acceptor acceptor;

public:
    explicit Server(unsigned short port)
    : acceptor(io_context, {tcp::v4(), port}) {}

    void start() {
        asio::co_spawn(io_context,
            [this]() -> asio::awaitable<void> {
                try {
                    for (;;) {
                        tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
                        for (std::shared_ptr handler : handlers) {
                            asio::co_spawn(
                                acceptor.get_executor(),
                                handler->handle_client(std::move(socket)),
                                asio::detached);
                        }
                    }
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                }
            },
            asio::detached);

        std::cout << "Serveur lancé sur le port " << acceptor.local_endpoint().port() << std::endl;
        io_context.run();
    }

    void add_handler(std::shared_ptr<AbstractHandeler> handler) {
        handlers.push_back(handler);
    }

    template <typename... Args>
        requires (std::convertible_to<Args, std::shared_ptr<AbstractHandeler>> && ...)
    void add_handlers(Args... handlers) {
        (add_handler(handlers), ...);
    }

    void remove_handler(const std::shared_ptr<AbstractHandeler>* handler) {
        int index;
        bool found = false;

        for (int i = 0; i < handlers.size() && !found; i++) {
            if (handlers[i] == *handler) {
                index = i;
                found = true;
            }
        }

        if (found) handlers.erase(handlers.begin() + index);
    }
};