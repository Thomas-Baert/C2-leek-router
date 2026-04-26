#include "network/server.hpp"
#include "network/handelers/Rebound.hpp"

int main() {
    const auto rebound = std::make_shared<Rebound>(12000);
    const auto rebound2 = std::make_shared<Rebound>(12002);
    auto server = Server(12001);
    server.add_handlers(rebound, rebound2);
    server.start(); 
}