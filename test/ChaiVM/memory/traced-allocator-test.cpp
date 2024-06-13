#include "ChaiVM/memory/linear-buffer.hpp"
#include "ChaiVM/memory/traced-allocator.hpp"
#include "ChaiVM/types.hpp"
#include <gtest/gtest.h>

using namespace chai::memory;

class LinearAllocatorTest : public ::testing::Test {
protected:
    static constexpr size_t BUFFER_SIZE = 2048;
};

TEST_F(LinearAllocatorTest, Array) {
    size_t n = 5;
    TracedByteAllocator allocator{BUFFER_SIZE};
    auto *buf = static_cast<chai::chsize_t *>(
        allocator.allocate(n * sizeof(chai::chsize_t)));
    auto *arr = new (buf) chai::chsize_t[n];
    for (size_t i = 0; i < n; ++i) {
        arr[i] = i;
    }
    for (size_t i = 0; i < n; ++i) {
        EXPECT_EQ(arr[i], i);
    }
    EXPECT_EQ(allocator.allocated(), n * sizeof(chai::chsize_t));
    EXPECT_EQ(allocator.allocations().size(), 1);
    EXPECT_EQ(allocator.allocations().back().isFree, false);
    EXPECT_EQ(allocator.allocations().back().size, n * sizeof(chai::chsize_t));
    EXPECT_EQ(allocator.allocations().back().ptr, arr);

    allocator.deallocate(arr, n);
}

TEST_F(LinearAllocatorTest, MultipleAllocs) {
    std::vector<size_t> sizes = {4, 8, 15, 16, 23, 42, 100};
    size_t sum_size = 0;
    TracedByteAllocator allocator{BUFFER_SIZE};
    for (auto size : sizes) {
        sum_size += size;
        chai::chsize_t *buf = static_cast<chai::chsize_t *>(
            allocator.allocate(size * sizeof(chai::chsize_t)));
        chai::chsize_t *arr = new (buf) chai::chsize_t[size];
        for (size_t i = 0; i < size; ++i) {
            arr[i] = i;
        }
        for (size_t i = 0; i < size; ++i) {
            EXPECT_EQ(arr[i], i);
        }
    }
    EXPECT_EQ(allocator.allocated(), sum_size * sizeof(chai::chsize_t));
    EXPECT_EQ(allocator.allocations().size(), sizes.size());
    size_t iter = 0;
    for (auto &alloc : allocator.allocations()) {
        EXPECT_EQ(alloc.isFree, false);
        EXPECT_EQ(alloc.size, sizes[iter] * sizeof(chai::chsize_t));
        iter++;
    }
}

TEST_F(LinearAllocatorTest, BadAlloc) {
    TracedByteAllocator allocator{BUFFER_SIZE};
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

    EXPECT_THROW(auto *buf = static_cast<chai::chsize_t *>(
                     allocator.allocate(BUFFER_SIZE)),
                 std::bad_alloc);
#pragma GCC diagnostic pop
}
