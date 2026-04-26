#pragma once
#include <array>
#include <algorithm>
#include <vector>
#include "config.h"


class RingBuffer {
  std::atomic<size_t> writing_ptr;
  std::atomic<size_t> reading_ptr;
  std::array<char, CAPACITY> buffer{};

public:
  RingBuffer() {
    buffer = std::array<char, CAPACITY>();
    writing_ptr = 0;
    reading_ptr = 0;
  }

  size_t write(const char* data, const size_t size) {
    const size_t free_space = (reading_ptr + CAPACITY - writing_ptr - 1) % CAPACITY;
    const size_t to_write = std::min(size, free_space);
    size_t wp = writing_ptr;

    for (size_t i = 0; i < to_write; i++) {
      buffer[wp] = data[i];
      wp = (wp + 1) % CAPACITY;
    }

    // On publie tout d'un coup
    writing_ptr.store(wp, std::memory_order_release);
    return to_write;
  }

  size_t read(char* read_buffer, const size_t len, const size_t cursor) const {
    const size_t total_available = (writing_ptr + CAPACITY - reading_ptr) % CAPACITY;
    if (cursor >= total_available) return 0;

    const size_t available_from_cursor = total_available - cursor;
    const size_t to_read = std::min(len, available_from_cursor);

    for (unsigned long i = (reading_ptr + cursor) % CAPACITY, j = 0; j < to_read && i != writing_ptr; j ++) {
      read_buffer[j] = buffer[i];
      i = (i + 1) % CAPACITY;
    }
    return to_read;
  }

  void update_reading_pointer(const size_t new_pointer) {
    reading_ptr.store(new_pointer, std::memory_order_release);
  }

  std::array<char, CAPACITY> get_buffer() const {
    return buffer;
  }

  size_t get_reading_ptr() const {
    return reading_ptr.load(std::memory_order_release);
  }
};