#pragma once

#include <list>

#include "ChaiVM/memory/free-list-trash/free-list-allocator.hpp"
#include "ChaiVM/types.hpp"

namespace chai::memory {

struct AllocationInfo final {
    void *ptr = nullptr;
    size_t size = 0;
    bool isFree = true;
};

class TracedByteAllocator final {
public:
    TracedByteAllocator(size_t size)
        : allocator_(size, FreeListAllocator::PlacementPolicy::FIND_FIRST) {
        allocator_.Init();
    }

    void *allocate(size_t bytes) {
        void *out = allocator_.Allocate(bytes, sizeof(chai::chsize_t));

        if (out != nullptr) {
            allocations_.push_back(AllocationInfo{out, bytes, false});
            allocated_ += bytes;
        } else {
            throw std::bad_alloc();
        }

        return out;
    }
    void deallocate(void *p, size_t bytes) {
        allocator_.Free(p);
        allocated_ -= bytes;
        // allocations_ are changed by GC
    }

    auto &allocated() noexcept { return allocated_; }
    auto &allocations() noexcept { return allocations_; }

private:
    size_t allocated_ = 0;
    std::list<AllocationInfo> allocations_;
    FreeListAllocator allocator_;
};

} // namespace chai::memory
