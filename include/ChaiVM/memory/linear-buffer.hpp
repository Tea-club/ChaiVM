#pragma once

#include <cstdlib>
#include <iostream>

#include "ChaiVM/utils/non-copyable.hpp"

namespace chai::memory {

class LinearBuffer : public INonCopyable {
public:
    explicit LinearBuffer(size_t sz);
    LinearBuffer(LinearBuffer &&other) noexcept;
    LinearBuffer &operator=(LinearBuffer &&other) noexcept;
    ~LinearBuffer();

    size_t size() const;
    size_t offset() const;
    void *currentPosition() const;
    void shiftOffset(size_t n);

private:
    size_t size_ = 0;
    size_t offset_ = 0;
    char *buf_ = nullptr;
};

} // namespace chai::memory
