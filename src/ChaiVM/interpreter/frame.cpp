#include <cassert>

#include "ChaiVM/interpreter/frame.hpp"

namespace chai::interpreter {

Frame::Frame(Frame *prev, const Function &func, memory::LinearBuffer &buffer)
    : func_(func), alloc_(memory::LinearAllocator<chsize_t>{buffer}),
      prev_(prev), regsize_(func.numRegs), registers_(func.numRegs, 0, alloc_) {}

void Frame::copyLastRegs() {
    assert(prev_ != nullptr);
    size_t nargs = func_.numArgs;
    assert(nargs <= regsize_);
    assert(nargs <= prev_->regsize_);
    for (size_t i = 0; i < nargs; ++i) {
        registers_[regsize_ - 1 - i] = prev_->registers_[prev_->regsize_ - 1 - i];
    }
}

chai::chsize_t &Frame::operator[](size_t n) & {
    assert(n < regsize_);
    return registers_[n];
}
const chsize_t &Frame::operator[](size_t n) const & {
    assert(n < regsize_);
    return registers_[n];
}

/**
 * Get state.
 * @return
 */
std::vector<chsize_t> Frame::copyState() {
    std::vector<chsize_t> ret{registers_.size()};
    for (auto item : registers_) {
        ret.push_back(item);
    }
    return ret;
}

Frame *Frame::back() { return prev_; }

} // namespace chai::interpreter
