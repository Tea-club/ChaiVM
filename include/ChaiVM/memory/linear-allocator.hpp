#pragma once

#include <cstdlib>
#include <new>
#include <numeric>

#include "ChaiVM/utils/non-copyable.hpp"

namespace chai::memory {

template<class T>
struct LinearAllocator : INonCopyable {
    typedef T value_type;

    LinearAllocator() = default;

    template<class U>
    constexpr LinearAllocator(const LinearAllocator<U>&) noexcept = delete;

    T* allocate(std::size_t n) {
        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();

        if (auto p = static_cast<T*>(std::malloc(n * sizeof(T)))) {
            report(p, n);
            return p;
        }

        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t n) noexcept {
        std::free(p);
    }
private:
};

//TODO
template<class T, class U>
bool operator==(const LinearAllocator <T>&, const LinearAllocator <U>&) { return true; }

//TODO
template<class T, class U>
bool operator!=(const LinearAllocator <T>&, const LinearAllocator <U>&) { return false; }

}