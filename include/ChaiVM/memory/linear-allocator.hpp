#pragma once

#include <cstdlib>
#include <new>
#include <numeric>

#include "ChaiVM/utils/non-copyable.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include "ChaiVM/memory/allocator.hpp"

namespace chai::memory {

template<class T>
class LinearAllocator : public Allocator<T>, public INonCopyable {
public:
    using value_type = T;

    explicit LinearAllocator(LinearBuffer &buffer) : buffer_(buffer) {}

//    template<class U>
//    constexpr LinearAllocator(const LinearAllocator<U>&) noexcept = delete;

    /**
     * @todo #32:60min Use placement new for objects in allocate and call their
     * destructors in deallocate. Create a hash map with pointers to objects and
     * allocated size.
     */
    T* allocate(std::size_t n) override {
        if (n > (buffer_.size() - buffer_.offset()) / sizeof(T)) {
            throw std::bad_array_new_length();
        }
        void *current = buffer_.currentPosition();
        buffer_.shiftOffset(n * sizeof(T));
        return reinterpret_cast<T *>(current);
    }
    void deallocate(T* p, std::size_t n) override {}
private:
    LinearBuffer &buffer_;
};

//TODO
template<class T, class U>
bool operator==(const LinearAllocator <T>&, const LinearAllocator <U>&) { return true; }

//TODO
template<class T, class U>
bool operator!=(const LinearAllocator <T>&, const LinearAllocator <U>&) { return false; }

}  // namespace chai::memory
