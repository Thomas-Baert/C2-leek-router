#pragma once
#include "ring_buffer.hpp"
#include <asio.hpp>
#include "config.h"

using asio::ip::tcp;

class ConcurrentSocket {
  RingBuffer ring_buffer;
  tcp::socket socket;
  std::array<std::atomic_size_t, MAX_CLIENT_CONNECTIONS> connections;
  size_t current_connections;
  std::mutex connections_mutex;

public:
  explicit ConcurrentSocket(tcp::socket& socket): socket(std::move(socket)) {
    connections = std::array<std::atomic_size_t, MAX_CLIENT_CONNECTIONS>();
    current_connections = 0;
  }

  size_t connect() {
    std::lock_guard lock(connections_mutex);
    const size_t new_id = current_connections;
    if (new_id >= MAX_CLIENT_CONNECTIONS) return -1;
    current_connections ++;
    connections[new_id] = 0;
    return new_id;
  }

  size_t read(const int id, const size_t len, char buffer[]) {
    std::lock_guard lock(connections_mutex);
    const size_t number_read = ring_buffer.read(buffer, len, connections[id]);
    connections[id] += number_read;
    ring_buffer.update_reading_pointer((ring_buffer.get_reading_ptr() + connections[get_id_last()]) % CAPACITY);
    if (number_read > 0) update_cursors();
    return number_read;
  }

  void free_connection(const size_t id) {
    std::lock_guard lock(connections_mutex);
    connections[id] = CAPACITY + 1;
  }

private:
  size_t get_id_last() const {
    size_t id = 0;
    size_t cursor = CAPACITY;

    for (int i = 0; i < current_connections; ++i) {
      if (connections[i] < cursor) {
        cursor = connections[i];
        id = i;
      }
    }
    return id;
  }

  void update_cursors() {
    const size_t cursor_last = connections[get_id_last()];
    for (int i = 0; i < current_connections; ++i) {
      connections[i] -= cursor_last;
    }
  }

  // Threaded routine. It updates the buffer with the sockets messages
  /*
  void routine() {
  }*/
};