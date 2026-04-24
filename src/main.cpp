#include "network/server.hpp"
#include "network/handelers/Rebound.hpp"

int main(int argc, char** argv) {
    auto rebound = std::make_shared<Rebound>(12000);
    auto rebound2 = std::make_shared<Rebound>(12002);
    Server server = Server(12001);
    server.add_handlers(rebound, rebound2);
    server.start(); 
}