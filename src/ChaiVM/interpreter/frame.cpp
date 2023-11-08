#include <cassert>

#include "ChaiVM/interpreter/frame.hpp"

namespace chai::interpreter {

Frame::Frame(Frame *prev, const Function &func, memory::LinearBuffer &buffer)
    : func_(func), alloc_(memory::LinearAllocator<chsize_t>{buffer}),
      prev_(prev), regsize(func.numRegs), registers_(func.numRegs, 0, alloc_) {}

void Frame::copyLastRegs() {
    assert(prev_ != nullptr);
    size_t nargs = func_.numArgs;
    assert(nargs <= regsize);
    assert(nargs <= prev_->regsize);
    for (size_t i = 0; i < nargs; ++i) {
        registers_[regsize - 1 - i] = prev_->registers_[prev_->regsize - 1 - i];
    }
}

} // namespace chai::interpreter
