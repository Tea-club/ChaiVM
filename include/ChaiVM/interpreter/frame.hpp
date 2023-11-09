#pragma once

#include "ChaiVM/interpreter/code-manager/func-struct.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"

namespace chai::interpreter {

class Frame {

public:
    Frame(Frame *prev, const Function &func, memory::LinearBuffer &buffer);

    void passArgs();

    chai::chsize_t &operator[](size_t n) &;
    const chsize_t &operator[](size_t n) const &;

    /**
     * Get state.
     * @return state.
     */
    std::vector<chsize_t> copyState();

    Frame *back();

public:
    Function const &func_;
    chsize_t pc_;

private:
    Frame *prev_;
    size_t regsize_;
    std::vector<chsize_t, memory::LinearAllocator<chsize_t>> registers_;
};

} // namespace chai::interpreter
