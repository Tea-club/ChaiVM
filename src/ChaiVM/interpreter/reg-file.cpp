#include "ChaiVM/interpreter/reg-file.hpp"
#include <cassert>

chai::chsize_t &chai::interpreter::RegisterFile::operator[](int n) & {
    assert(n <= Size);
    return registers_[n];
}
chai::chsize_t chai::interpreter::RegisterFile::getAcc() const { return acc_; }
void chai::interpreter::RegisterFile::setAcc(chai::chsize_t value) {
    acc_ = value;
}
chai::chsize_t chai::interpreter::RegisterFile::getPc() const { return pc_; }
void chai::interpreter::RegisterFile::setPc(chai::chsize_t value) {
    pc_ = value;
}
