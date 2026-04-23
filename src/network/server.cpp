#include "handelers/abstract_handeler.cpp"
#include <memory>
#include <asio.hpp>
#include <iostream>

using namespace std;
using asio::ip::tcp;

class Server {
    // Relays REQCLIENT to the back-end client
    // Must not be exposed directly to the internet
    std::unique_ptr<AbstractHandeler> current_handler;
    asio::io_context io_context;
    tcp::acceptor acceptor;

    public:
        Server(short port, std::unique_ptr<AbstractHandeler> client_handler): acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
            this->current_handler = std::move(client_handler);
        }

        void start() {
            std::cout << "Serveur en écoute sur le port " << acceptor.local_endpoint().port() << "..." << std::endl;

            while (true) {
                tcp::socket socket(io_context);
                acceptor.accept(socket);
                std::cout << "Nouveau client : " << socket.remote_endpoint() << std::endl;

                if (this->current_handler) {
                    this->current_handler->handle_client();
                }
            }
        }
};