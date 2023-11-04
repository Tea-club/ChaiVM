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

class FrameStub {
public:
    using RegType = int;
    size_t nregs_ = 0;
    std::vector<RegType, LinearAllocator<RegType>> regs_;
    static constexpr int DFT = -1;
    FrameStub(size_t nregs, LinearBuffer &buffer)
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
    for (auto &item : vec) {
        EXPECT_EQ(item.stub_, Stub::DFT);
    }
}
TEST_F(LinearAllocatorTest, StdContainers) {
    size_t n = 10;
    LinearAllocator<Stub> allocator{buffer_};
    std::vector<Stub, decltype(allocator)> vec(n, allocator);
    for (auto &item : vec) {
        item = Stub(42);
    }
    for (auto &item : vec) {
        EXPECT_EQ(item.stub_, 42);
    }
}
TEST_F(LinearAllocatorTest, FrameContainersDefault) {
    size_t n = 10;
    size_t nregs = 16;
    LinearAllocator<FrameStub> allocator{buffer_};
    std::vector<FrameStub, decltype(allocator)> vec(
        n, FrameStub(nregs, buffer_), allocator);
    for (auto &item : vec) {
        EXPECT_EQ(item.nregs_, nregs);
        EXPECT_EQ(item.regs_.size(), nregs);
        for (auto &r : item.regs_) {
            EXPECT_EQ(r, FrameStub::DFT);
        }
    }
}
TEST_F(LinearAllocatorTest, FrameContainers) {
    size_t n = 8;
    size_t nregs = 16;
    LinearAllocator<FrameStub> allocator{buffer_};
    std::vector<FrameStub, decltype(allocator)> vec(
        n, FrameStub(nregs, buffer_), allocator);
    for (auto &item : vec) {
        for (auto &r : item.regs_) {
            r = 426;
        }
        item.regs_.push_back(426);
        item.nregs_++;
    }
    for (auto &item : vec) {
        EXPECT_EQ(item.nregs_, nregs + 1);
        EXPECT_EQ(item.regs_.size(), nregs + 1);
        for (auto &r : item.regs_) {
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
TEST_F(LinearAllocatorTest, FrameContainersSingleAllocation) {
    size_t nregs = 39;
    LinearAllocator<FrameStub> allocator{buffer_};
    FrameStub *buf = allocator.allocate(1);
    FrameStub *inst = new (buf) FrameStub(nregs, buffer_);
    inst->regs_[nregs - 1] = 20;
    EXPECT_EQ(buffer_.offset(),
              sizeof(FrameStub) + nregs * sizeof(FrameStub::RegType));
    EXPECT_EQ(inst->regs_[nregs - 1], 20);
}
TEST_F(LinearAllocatorTest, FrameContainersAllocation) {
    size_t n = 8;
    size_t nregs = 16;
    LinearAllocator<FrameStub> allocator{buffer_};
    std::vector<FrameStub, decltype(allocator)> vec(n, {nregs, buffer_},
                                                    allocator);
    /**
     * An extra `regs_` vector (the last term
     * `nregs * sizeof(FrameLikeStub::RegType)`) appears due to extra call of
     * FrameStub constructor when constructing a `vec`
     */
    EXPECT_EQ(buffer_.offset(),
              n * (sizeof(FrameStub) + nregs * sizeof(FrameStub::RegType)) +
                  nregs * sizeof(FrameStub::RegType));
}

TEST_F(LinearAllocatorTest, BadArrayNewLength) {
    size_t n = BUFFER_SIZE / sizeof(Stub) + 1;
    LinearAllocator<Stub> allocator{buffer_};
    EXPECT_THROW(allocator.allocate(n), std::bad_array_new_length);
}
