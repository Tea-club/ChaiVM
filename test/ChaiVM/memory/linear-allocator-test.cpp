#include "ChaiVM/memory/linear-allocator.hpp"
#include <gtest/gtest.h>

using namespace chai::memory;

class LinearAllocatorTest : public ::testing::Test {
protected:
    static constexpr size_t BUFFER_SIZE = 2048;
    LinearBuffer buffer_ = LinearBuffer(BUFFER_SIZE);
};

class Stub {
public:
    static constexpr int DFT = -1;
    explicit Stub(int num) : stub_(num) {}
    Stub() {}

    int stub_ = DFT;
};

class FrameLikeStub {
public:
    using RegType = int;
    size_t nregs_ = 0;
    std::vector<RegType, LinearAllocator<RegType>> regs_;
    static constexpr int DFT = -1;
    FrameLikeStub(size_t nregs, LinearBuffer &buffer)
        : nregs_(nregs), regs_(nregs_, DFT, LinearAllocator<RegType>(buffer)) {}
};

TEST_F(LinearAllocatorTest, Primitives) {
    size_t n = 5;
    LinearAllocator<int> allocator{buffer_};
    int *buf = allocator.allocate(n);
    int *arr = new (buf) int[n];
    for (size_t i = 0; i < n; ++i) {
        arr[i] = i;
    }
    for (size_t i = 0; i < n; ++i) {
        EXPECT_EQ(arr[i], i);
    }
}
TEST_F(LinearAllocatorTest, Classes) {
    size_t n = 1;
    LinearAllocator<Stub> allocator{buffer_};
    Stub *buf = allocator.allocate(n);
    Stub *inst = new (buf) Stub;
    inst->stub_ = 10;
    EXPECT_EQ(inst->stub_, 10);
    inst->~Stub();
}
TEST_F(LinearAllocatorTest, StdContainersDefault) {
    size_t n = 10;
    LinearAllocator<Stub> allocator{buffer_};
    std::vector<Stub, decltype(allocator)> vec(n, allocator);
    for (auto &e : vec) {
        EXPECT_EQ(e.stub_, Stub::DFT);
    }
}
TEST_F(LinearAllocatorTest, StdContainers) {
    size_t n = 10;
    LinearAllocator<Stub> allocator{buffer_};
    std::vector<Stub, decltype(allocator)> vec(n, allocator);
    for (auto &e : vec) {
        e = Stub(42);
    }
    for (auto &e : vec) {
        EXPECT_EQ(e.stub_, 42);
    }
}
TEST_F(LinearAllocatorTest, FrameLikeContainersDefault) {
    size_t n = 10;
    size_t nregs = 16;
    LinearAllocator<FrameLikeStub> allocator{buffer_};
    std::vector<FrameLikeStub, decltype(allocator)> vec(
        n, FrameLikeStub(nregs, buffer_), allocator);
    for (auto &e : vec) {
        EXPECT_EQ(e.nregs_, nregs);
        EXPECT_EQ(e.regs_.size(), nregs);
        for (auto &r : e.regs_) {
            EXPECT_EQ(r, FrameLikeStub::DFT);
        }
    }
}
TEST_F(LinearAllocatorTest, FrameLikeContainers) {
    size_t n = 8;
    size_t nregs = 16;
    LinearAllocator<FrameLikeStub> allocator{buffer_};
    std::vector<FrameLikeStub, decltype(allocator)> vec(
        n, FrameLikeStub(nregs, buffer_), allocator);
    for (auto &e : vec) {
        for (auto &r : e.regs_) {
            r = 426;
        }
        e.regs_.push_back(426);
        e.nregs_++;
    }
    for (auto &e : vec) {
        EXPECT_EQ(e.nregs_, nregs + 1);
        EXPECT_EQ(e.regs_.size(), nregs + 1);
        for (auto &r : e.regs_) {
            EXPECT_EQ(r, 426);
        }
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

TEST_F(LinearAllocatorTest, FrameLikeContainersAllocation) {
    size_t n = 1;
    size_t nregs = 2;
    LinearAllocator<FrameLikeStub> allocator{buffer_};
    std::vector<FrameLikeStub, decltype(allocator)> vec(
        n, FrameLikeStub(nregs, buffer_), allocator);

    /**
     * @todo #32:60min Find out is this an error, and where an additional vector
     * is constructed. IDK why does an extra vector appears (the last term
     * `nregs * sizeof(FrameLikeStub::RegType)`)
     */
    EXPECT_EQ(buffer_.offset(), n * (sizeof(FrameLikeStub) +
                                     nregs * sizeof(FrameLikeStub::RegType)) +
                                    nregs * sizeof(FrameLikeStub::RegType));
}

TEST_F(LinearAllocatorTest, BadArrayNewLength) {
    size_t n = BUFFER_SIZE / sizeof(Stub) + 1;
    LinearAllocator<Stub> allocator{buffer_};
    EXPECT_THROW(allocator.allocate(n), std::bad_array_new_length);
}
