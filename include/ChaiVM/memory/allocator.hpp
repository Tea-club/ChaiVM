#pragma once

#include <cstdlib>

namespace chai::memory {

template <class T> class IAllocator {
public:
    using value_type = T;
    IAllocator() noexcept = default;

    /**
     * Allocate memory for n T-objects.
     * @param n Number of objects.
     * @return Ponter to start of allocated mem.
     */
    virtual T *allocate(size_t n) = 0;

    /**
     * Deallocate memory.
     * @param p Where deallocate from.
     * @param n Number of objects to be deallocated.
     */
    virtual void deallocate(T *p, size_t n) = 0;
    virtual ~IAllocator() {}
};

} // namespace chai::memory
