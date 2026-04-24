#include <string>

using namespace std;

class Peer {
public: 
    int id;
    string ip;

    Peer(int id, string ip) {
        this->id = id;
        this->ip = ip;
    }

    string get_ip() const {
        return ip;
    }

    int get_id() const {
        return id;
    }
};