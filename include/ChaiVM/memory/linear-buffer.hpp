#pragma once

#include <cstdlib>
#include <iostream>
#include <memory_resource>

#include "ChaiVM/utils/non-copyable.hpp"

namespace chai::memory {

class LinearBuffer final : public INonCopyable,
                           public std::pmr::memory_resource {
public:
    LinearBuffer(size_t sz);
    LinearBuffer(LinearBuffer &&other) = delete;
    LinearBuffer &operator=(LinearBuffer &&other) = delete;
    ~LinearBuffer() override;

    size_t size() const;
    size_t offset() const;
    void *currentPosition() const;

private:
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
    void *do_allocate(size_t n, size_t alignment = sizeof(uint64_t)) override;
#pragma GCC diagnostic pop
    void do_deallocate(void * /*unused*/, size_t n, size_t /*unused*/) override;
    bool do_is_equal(const memory_resource &other) const noexcept override;

private:
    char *buf_ = nullptr;
    char *curr_ = nullptr;
    size_t free_ = 0;
};

} // namespace chai::memory
