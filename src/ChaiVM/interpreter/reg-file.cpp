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
    printf("pc = %lu, acc = %ld = %lf\n", pc_, std::bit_cast<int64_t>(acc_),
           std::bit_cast<double>(acc_));
    for (int i = 0; i < Size; ++i) {
        if (registers_[i] != 0) {
            printf("rf[%i] = %ld = %lf\n", i,
                   std::bit_cast<int64_t>(registers_[i]),
                   std::bit_cast<double>(registers_[i]));
        }
    }
    printf("\n\n");
}
} // namespace chai::interpreter
