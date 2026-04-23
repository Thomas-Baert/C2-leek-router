#include "handelers/abstract_handeler.hpp"
#include <memory>
#include <asio.hpp>
#include <iostream>

using namespace std;
using asio::ip::tcp;

class Server {
    std::shared_ptr<AbstractHandeler> current_handler;
    asio::io_context io_context;
    tcp::acceptor acceptor;

    public:
        Server(unsigned short port, std::shared_ptr<AbstractHandeler> client_handler)
        : current_handler(std::move(client_handler)), acceptor(io_context, {tcp::v4(), port}) {}

        void start() {
            shared_ptr<AbstractHandeler> handler = current_handler;

            asio::co_spawn(io_context,
                [this, handler]() -> asio::awaitable<void> {
                    try {
                        for (;;) {
                            tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
                            asio::co_spawn(
                                acceptor.get_executor(),
                                handler->handle_client(std::move(socket)),
                                asio::detached);
                        }
                    } catch (std::exception &e) {
                        std::cerr << e.what() << std::endl;
                    }
                },
                asio::detached);

            std::cout << "Serveur lancé sur le port " << acceptor.local_endpoint().port() << std::endl;
            io_context.run();
        }
};