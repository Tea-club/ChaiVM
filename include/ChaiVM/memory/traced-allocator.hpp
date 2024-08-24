#pragma once

#include <list>
#include <exception>

#include "ChaiVM/memory/free-list-trash/free-list-allocator.hpp"
#include "ChaiVM/types.hpp"

namespace chai::memory {

class out_of_memory_exception final : public std::bad_alloc {
public:
    const char* what() const noexcept {
        return msg;
    }
private:
    static constexpr const char* msg = "VM is out of memory";
};

struct AllocationInfo final {
    void *ptr = nullptr;
    size_t size = 0;
    bool isFree = true;
};

class TracedByteAllocator final {
public:
    TracedByteAllocator(size_t size)
        : size_(size), allocator_(size, FreeListAllocator::PlacementPolicy::FIND_FIRST) {
        allocator_.Init();
    }

    void *allocate(size_t bytes) {
        void *out = allocator_.Allocate(bytes, sizeof(chai::chsize_t));

        if (out != nullptr) {
            allocations_.push_back(AllocationInfo{out, bytes, false});
            allocated_ += bytes;
        } else {
            throw out_of_memory_exception();
        }

        return out;
    }
    void deallocate(void *p, size_t bytes) {
        allocator_.Free(p);
        allocated_ -= bytes;
        // allocations_ are changed by GC
    }

    size_t &allocated() { return allocated_; }
    size_t const &allocated() const noexcept { return allocated_; }
    auto &allocations() { return allocations_; }
    size_t size() { return size_; }

private:
    size_t size_ = 0;
    size_t allocated_ = 0;
    std::list<AllocationInfo> allocations_;
    FreeListAllocator allocator_;
};

} // namespace chai::memory
