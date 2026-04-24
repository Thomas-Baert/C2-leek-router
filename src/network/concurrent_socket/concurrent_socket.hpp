#pragma once
#include <array>
#include <algorithm>
#include <vector>

class RingBuffer {
    static constexpr size_t CAPACITY = 1024 * 1024; // 1MB

    std::array<char, CAPACITY> buffer;

    size_t write_pos = 0;
    size_t size = 0;

public:
    size_t write(const char* data, size_t len) {
        size_t written = 0;

        while (written < len && size < CAPACITY) {
            buffer[write_pos] = data[written];

            write_pos = (write_pos + 1) % CAPACITY;
            written++;
            size++;
        }

        return written;
    }

    size_t read(char* out,
                size_t len,
                size_t cursor) const {

        if (cursor >= size)
            return 0;

        size_t available = size - cursor;
        size_t to_read = std::min(len, available);

        size_t pos = (write_pos + CAPACITY - size + cursor) % CAPACITY;

        for (size_t i = 0; i < to_read; i++) {
            out[i] = buffer[(pos + i) % CAPACITY];
        }

        return to_read;
    }

    size_t available() const {
        return size;
    }
};