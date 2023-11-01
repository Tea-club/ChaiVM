#pragma once

#include <cstdlib>

namespace chai::memory {

template <class T> class Allocator {
public:
    using value_type = T;
    Allocator() noexcept = default;
//    template <class U> Allocator(const Allocator<U> &) noexcept {}
    virtual T *allocate(size_t n) = 0;
    virtual void deallocate(T *p, size_t n) = 0;
    virtual ~Allocator() = 0;
};

template <class T, class U>
constexpr bool operator==(const Allocator<T> &, const Allocator<U> &) noexcept;

template <class T, class U>
constexpr bool operator!=(const Allocator<T> &, const Allocator<U> &) noexcept;

}  // namespace chai::memory
