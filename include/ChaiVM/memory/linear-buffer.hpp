#pragma once

#include <cstdlib>
#include <iostream>

#include "ChaiVM/utils/non-copyable.hpp"

namespace chai::memory {

class LinearBuffer : public INonCopyable {
public:
    explicit LinearBuffer(size_t sz) : size_(sz), buf_(new char[size_]) {}
    LinearBuffer(LinearBuffer &&other) noexcept {
        std::swap(size_, other.size_);
        std::swap(buf_, other.buf_);
    }
    LinearBuffer &operator=(LinearBuffer &&other) noexcept {
        std::swap(size_, other.size_);
        std::swap(buf_, other.buf_);
        return *this;
    }
    ~LinearBuffer() { delete[] buf_; }

    size_t size() const { return size_; }
    size_t offset() const { return offset_; }
    void *currentPosition() { return buf_ + offset_; }
    void shiftOffset(size_t n) { offset_ += n; }

private:
    size_t size_ = 0;
    size_t offset_ = 0;
    char *buf_ = nullptr;
};

}  // namespace chai::memory
