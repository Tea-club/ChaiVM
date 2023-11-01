#include <gtest/gtest.h>
#include "ChaiVM/memory/linear-allocator.hpp"

using namespace chai::memory;

constexpr size_t BUFFER_SIZE = 1024;

TEST(LinearAllocatorTest, primitive) {
    LinearBuffer buffer(BUFFER_SIZE);
    LinearAllocator<int> allocator(buffer);
    int* arr = allocator.allocate(5);
}
