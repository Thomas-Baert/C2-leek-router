#include <string>

using namespace std;

class Peer {
public: 
    int id;
    string ip;

public: 
    Peer(int id, string ip) {
        this->id = id;
        this->ip = ip;
    }
};