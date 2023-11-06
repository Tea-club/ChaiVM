#pragma once

namespace chai::interpreter {

class Frame {

public:
    Frame(Frame *prev, const Function &func, memory::LinearBuffer &buffer)
        : func_(func), alloc_(memory::LinearAllocator<chsize_t>{buffer}),
          prev_(prev),
          regsize(func.num_regs), registers_{func.num_regs, alloc_} {}

    ~Frame() { std::cout << "destructor of frame" << std::endl; }

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
