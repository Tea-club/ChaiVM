#pragma once

#include <cassert>
#include <cstdlib>
#include <new>
#include <numeric>

#include "ChaiVM/memory/allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include "ChaiVM/utils/non-copyable.hpp"

namespace chai::memory {

template <class T> class LinearAllocator : IAllocator<T> {
public:
    /**
     * Do not remove.
     */
    using value_type = T;

    explicit LinearAllocator(LinearBuffer &buffer) : buffer_(buffer) {}

    T *allocate(std::size_t n) override {
        if (n > (buffer_.size() - buffer_.offset()) / sizeof(T)) {
            throw std::bad_array_new_length();
        }
        return reinterpret_cast<T *>(buffer_.allocate(n * sizeof(T)));
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    void deallocate(T *p, std::size_t n) override {
        if (n > buffer_.offset() / sizeof(T)) {
            throw std::runtime_error("invalid deallocation size");
        }
        buffer_.deallocate(n * sizeof(T));
    }
#pragma GCC diagnostic pop

private:
    LinearBuffer &buffer_;
};

} // namespace chai::memory
