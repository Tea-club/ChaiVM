#pragma once

#include "ChaiVM/interpreter/code-manager/func-struct.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"

namespace chai::interpreter {

class Frame {

public:
    Frame(Frame *prev, const Function &func, memory::LinearBuffer &buffer);

    void copyLastRegs();

    chai::chsize_t &operator[](size_t n) & {
        assert(n < regsize);
        return registers_[n];
    }
    const chsize_t &operator[](size_t n) const & {
        assert(n < regsize);
        return registers_[n];
    }

    /**
     * Get state.
     * @return
     */
    std::vector<chsize_t> copyState() {
        std::vector<chsize_t> ret{registers_.size()};
        for (auto item : registers_) {
            ret.push_back(item);
        }
        return ret;
    }

    Frame *back() { return prev_; }

public:
    Function const &func_;
    chsize_t pc_;

private:
    memory::LinearAllocator<chsize_t> alloc_;
    Frame *prev_;
    size_t regsize;
    std::vector<chsize_t, memory::LinearAllocator<chsize_t>> registers_;
};

} // namespace chai::interpreter
