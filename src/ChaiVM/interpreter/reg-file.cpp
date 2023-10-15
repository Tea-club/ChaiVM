#include "ChaiVM/interpreter/reg-file.hpp"
#include <bit>
#include <cassert>
#include <iostream>

namespace chai::interpreter {

chai::chsize_t &RegisterFile::operator[](int n) & {
    assert(n <= Size);
    return registers_[n];
}
const chsize_t &RegisterFile::operator[](int n) const & {
    assert(n <= Size);
    return registers_[n];
}
chai::chsize_t &RegisterFile::pc() { return pc_; }
chai::chsize_t RegisterFile::pc() const { return pc_; }
chai::chsize_t &RegisterFile::acc() { return acc_; }
chai::chsize_t RegisterFile::acc() const { return acc_; }
RegisterFile::RegisterFile(chsize_t pc) : pc_(pc), acc_(0), registers_{} {}

void RegisterFile::dump() {
    std::cout << "pc = " << pc_ << ", acc = " << std::bit_cast<int64_t>(acc_)
              << " = " << std::bit_cast<double>(acc_) << std::endl;
    for (int i = 0; i < Size; ++i) {
        if (registers_[i] != 0) {
            std::cout << "rf[" << i
                      << "] = " << std::bit_cast<int64_t>(registers_[i])
                      << " = " << std::bit_cast<double>(registers_[i])
                      << std::endl;
        }
    }
    std::cout << std::endl;
}
} // namespace chai::interpreter
