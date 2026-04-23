#include <iostream>
#include "network/peer/peer_table.cpp"

int main(int argc, char** argv) {
    PeerTable peerTable;
    peerTable.add_peer(1, "127.0.0.1");
    peerTable.add_peer(2, "127.0.0.2");
    std::cout << peerTable << std::endl;
    return 0;
}