#include <gtest/gtest.h>
#include "ChaiVM/memory/linear-allocator.hpp"

using namespace chai::memory;

class LinearAllocatorTest : public ::testing::Test {
protected:
    static constexpr size_t BUFFER_SIZE = 1024;
    LinearBuffer buffer_ = LinearBuffer(BUFFER_SIZE);
};

class Stub {
public:
    explicit Stub(int num) : stub(num) {}
    Stub() {}
    int stub = 0;
};

TEST_F(LinearAllocatorTest, Primitives) {
    size_t n = 5;
    LinearAllocator<int> allocator{buffer_};
    int* buf = allocator.allocate(n);
    int* arr = new(buf) int[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = i;
    }
    for (int i = 0; i < n; ++i) {
        EXPECT_EQ(arr[i], i);
    }
}
TEST_F(LinearAllocatorTest, Classes) {
    size_t n = 1;
    LinearAllocator<Stub> allocator{buffer_};
    Stub* buf = allocator.allocate(n);
    Stub* inst = new(buf) Stub;
    inst->stub = 10;
    EXPECT_EQ(inst->stub, 10);
    inst->~Stub();
}
TEST_F(LinearAllocatorTest, StdContainers) {
    size_t n = 10;
    LinearAllocator<Stub> allocator{buffer_};
    std::vector<Stub, decltype(allocator)> vec(n, allocator);
    for (auto &e : vec) {
        e = Stub(42);
    }
    for (auto &e : vec) {
        EXPECT_EQ(e.stub, 42);
    }
}

TEST_F(LinearAllocatorTest, PrimitiveAllocation) {
    size_t n = 42;
    LinearAllocator<int> allocator{buffer_};
    allocator.allocate(n);
    EXPECT_EQ(buffer_.offset(), n * sizeof(int));
}
TEST_F(LinearAllocatorTest, ClassAllocation) {
    size_t n = 80;
    LinearAllocator<Stub> allocator{buffer_};
    allocator.allocate(n);
    EXPECT_EQ(buffer_.offset(), n * sizeof(Stub));
}
TEST_F(LinearAllocatorTest, StdContainersAllocation) {
    size_t n = 23;
    LinearAllocator<Stub> allocator{buffer_};
    std::vector<Stub, decltype(allocator)> vec(n, allocator);
    EXPECT_EQ(buffer_.offset(), n * sizeof(Stub));
}

TEST_F(LinearAllocatorTest, BasArrayNewLength) {
    size_t n = BUFFER_SIZE / sizeof(Stub) + 1;
    LinearAllocator<Stub> allocator{buffer_};
    EXPECT_THROW(allocator.allocate(n), std::bad_array_new_length);
}
