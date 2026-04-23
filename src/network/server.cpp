#include "handelers/abstract_handeler.cpp"

class Server {
    // Relays REQCLIENT to the back-end client
    // Must not be exposed directly to the internet
    private:
        int sock_fd;
        AbstractHandeler client_handler;
        struct sockaddr_in addr;
    public:
        Server(int port, AbstractHandeler client_handler) {
            this->client_handler = client_handler;
            this->addr.sin_family = AF_INET;
            this->addr.sin_port = port;
            this->addr.sin_addr.s_addr = INADDR_ANY;
            this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
            bind(this->sock_fd, (struct sockaddr *)&this->addr, sizeof(this->addr));
            listen(this->sock_fd, 10);
        }

        void start() {
            while (true) {
                accept(this->sock_fd, (struct sockaddr *)&this->addr, sizeof(this->addr));
                this->client_handler.handle_client();
            }
        }
};