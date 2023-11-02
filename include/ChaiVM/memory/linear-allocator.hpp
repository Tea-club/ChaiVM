#pragma once

#include <cstdlib>
#include <new>
#include <numeric>

#include "ChaiVM/memory/allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include "ChaiVM/utils/non-copyable.hpp"

namespace chai::memory {

template <class T> class LinearAllocator final : IAllocator<T> {
public:
    using value_type = T;

    explicit LinearAllocator(LinearBuffer &buffer) : buffer_(buffer) {}

    T *allocate(std::size_t n) override {
        if (n > (buffer_.size() - buffer_.offset()) / sizeof(T)) {
            throw std::bad_array_new_length();
        }
        void *current = buffer_.currentPosition();
        buffer_.shiftOffset(n * sizeof(T));
        return reinterpret_cast<T *>(current);
    }
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    void deallocate(T *p, std::size_t n) override {}
#pragma GCC diagnostic pop

private:
    LinearBuffer &buffer_;
};

} // namespace chai::memory
