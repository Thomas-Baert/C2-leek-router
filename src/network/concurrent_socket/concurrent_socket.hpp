#pragma once
#include "ring_buffer.hpp"
#include <asio.hpp>
#include "config.h"

using asio::ip::tcp;

class ConcurrentSocket {
  RingBuffer ring_buffer;
  tcp::socket socket;

  std::array<size_t, MAX_CLIENT_CONNECTIONS> connections{};
  std::array<bool, MAX_CLIENT_CONNECTIONS> is_free{};

  size_t current_connections;
  size_t freed_connections;

  std::mutex connections_mutex;

public:
  explicit ConcurrentSocket(tcp::socket& socket): socket(std::move(socket)) {
    connections = std::array<size_t, MAX_CLIENT_CONNECTIONS>();
    is_free = std::array<bool, MAX_CLIENT_CONNECTIONS>();
    current_connections = 0;
    freed_connections = 0;
  }

  int connect() {
    std::lock_guard lock(connections_mutex);

    for (int i = 0; i < current_connections; i++) {
      if (is_free[i]) {
        freed_connections--;
        is_free[i] = false;
        connections[i] = 0;
        return i;
      }
    }

    const int new_id = static_cast<int>(current_connections);
    if (new_id >= MAX_CLIENT_CONNECTIONS) return -1;
    current_connections ++;
    connections[new_id] = 0;
    return new_id;
  }

  int read(const int id, const size_t len, char buffer[]) {
    std::lock_guard lock(connections_mutex);
    const size_t number_read = ring_buffer.read(buffer, len, connections[id]);
    if (number_read == 0) return 0;
    connections[id] += number_read;
    const int id_slowest = get_id_last();
    if (id_slowest == -1) return -1;
    ring_buffer.update_reading_pointer((ring_buffer.get_reading_ptr() + connections[id_slowest]) % CAPACITY);
    update_cursors(id_slowest);
    return static_cast<int>(number_read);
  }

  void free_connection(const size_t id) {
    std::lock_guard lock(connections_mutex);
    connections[id] = CAPACITY + 1;
    is_free[id] = true;
    freed_connections++;
  }

  size_t get_number_of_connections() const {
    return current_connections - freed_connections;
  }

private:
  int get_id_last() const {
    int id = -1;
    size_t cursor = CAPACITY;

    for (int i = 0; i < current_connections; ++i) {
      if (connections[i] < cursor && !is_free[i]) {
        cursor = connections[i];
        id = i;
      }
    }
    return id;
  }

  void update_cursors(const size_t id_last) {
    const size_t cursor_last = connections[id_last];
    for (int i = 0; i < current_connections; ++i) {
      if (connections[i] <= CAPACITY) { // if active clients
            connections[i] -= cursor_last;
        }
    }
  }

  // Threaded routine. It updates the buffer with the sockets messages

  void routine() {
    auto data = std::vector<char>(CAPACITY);
    const char* data_ptr = data.data();
    for (;;) {
      asio::error_code ec;
      size_t number_read = socket.read_some(asio::buffer(data), ec);

      if (ec) break;
      if (number_read != 0) {
        size_t number_written = ring_buffer.write(data_ptr, number_read);
        size_t total_written = 0;

        while (number_written < number_read) {
          total_written += number_written;
          number_read -= number_written;
          number_written = ring_buffer.write(data_ptr + total_written, number_read);

          if (number_written < number_read) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
      }
    }
  }
};