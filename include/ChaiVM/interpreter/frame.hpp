#pragma once

#include "ChaiVM/interpreter/code-manager/func-struct.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include <bitset>

namespace chai::interpreter {

class Frame {

public:
    Frame(Frame *prev, const Function &func, memory::LinearBuffer &buffer);

    void passArgs();

    chai::chsize_t &operator[](size_t n) &;
    const chsize_t &operator[](size_t n) const &;
    size_t size() const;

    bool isRegisterReference(RegisterId reg_id) const;
    void setRegisterIsRef(RegisterId reg_id);

    /**
     * Get state.
     * @return state.
     */
    std::vector<chsize_t> copyState() const;

    Frame *back();
    const Frame *back() const;

public:
    Function const &func_;
    chsize_t pc_;

private:
    Frame *prev_;
    size_t regsize_;
    std::vector<chsize_t, memory::LinearAllocator<chsize_t>> registers_;
    // @todo #111:60min use custom allocator here
    std::bitset<256> isRegRef_;
};

} // namespace chai::interpreter
