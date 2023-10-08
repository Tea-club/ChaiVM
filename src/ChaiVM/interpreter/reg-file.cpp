#include "ChaiVM/interpreter/reg-file.hpp"
#include <cassert>

namespace chai::interpreter {

chai::chsize_t &RegisterFile::operator[](int n) & {
    assert(n <= Size);
    return registers_[n];
}
chai::chsize_t &RegisterFile::pc() { return pc_; }
chai::chsize_t RegisterFile::pc() const { return pc_; }
chai::chsize_t &RegisterFile::acc() { return acc_; }
chai::chsize_t RegisterFile::acc() const { return acc_; }
RegisterFile::RegisterFile(chsize_t pc) : pc_(pc) {}

}
