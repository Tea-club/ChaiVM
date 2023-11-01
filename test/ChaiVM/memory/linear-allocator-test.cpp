#include <gtest/gtest.h>
#include "ChaiVM/memory/linear-allocator.hpp"

using namespace chai::memory;

constexpr size_t BUFFER_SIZE = 1024;

TEST(LinearAllocatorTest, primitives) {
    LinearBuffer buffer(BUFFER_SIZE);
    LinearAllocator<int> allocator(buffer);
    size_t n = 5;
    int* buf = allocator.allocate(n);
    int* arr = new(buf) int[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(arr[i], i);
    }
}

TEST(LinearAllocatorTest, classes) {

}
