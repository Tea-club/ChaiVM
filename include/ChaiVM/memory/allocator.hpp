#pragma once

#include <cstdlib>

namespace chai::memory {

template <class T> class IAllocator {
public:
    using value_type = T;
    IAllocator() noexcept = default;
    virtual T *allocate(size_t n) = 0;
    virtual void deallocate(T *p, size_t n) = 0;
    virtual ~IAllocator() {}
};

}  // namespace chai::memory
