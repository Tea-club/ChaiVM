#pragma once

namespace chai::interpreter {

class Frame {

public:
    Frame(Frame * prev, const Function & func, memory::LinearBuffer& buffer):
        func_(func), prev_(prev), regsize(func.num_regs), registers_{func.num_regs, memory::LinearAllocator<chsize_t>{buffer}}  {
    }

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
        for (auto item: registers_) {
            ret.push_back(item);
        }
        return ret;
    }
public:
    Function const & func_;
private:
    Frame *prev_;
    size_t regsize;
    std::vector<chsize_t, memory::LinearAllocator<chsize_t>> registers_;
};

} // namespace chai::interpreter
