#include <iostream>
#include "network/server.hpp"
#include "network/handelers/Rebound.h"

int main(int argc, char** argv) {
    auto rebound = std::make_shared<Rebound>(12000);
    Server server = Server(12001, rebound);
    server.start();
}