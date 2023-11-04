#pragma once

namespace chai::interpreter {

template <typename Aloc>
class Frame {

private:
    Frame *prev;
    FunctionInfo *func;
    size_t regsize;
    std::vector<chsize_t, Aloc> regs;
};

} // namespace chai::interpreter
