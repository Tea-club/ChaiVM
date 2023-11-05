#include "ChaiVM/interpreter/reg-file.hpp"
#include <bit>
#include <cassert>
#include <iostream>

namespace chai::interpreter {

chai::chsize_t &RegisterFile::pc() { return pc_; }
chai::chsize_t RegisterFile::pc() const { return pc_; }
chai::chsize_t &RegisterFile::acc() { return acc_; }
chai::chsize_t RegisterFile::acc() const { return acc_; }
RegisterFile::RegisterFile(chsize_t pc) : acc_(0), pc_(pc), registers_{} {}

void RegisterFile::dump() {
    std::cout << "pc = " << pc_ << ", acc = " << std::bit_cast<int64_t>(acc_)
              << " = " << std::bit_cast<double>(acc_) << std::endl;
    for (size_t i = 0; i < Size; ++i) {
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
