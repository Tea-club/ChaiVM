#include "ChaiVM/interpreter/reg-file.hpp"
#include <cassert>

using namespace chai::interpreter;

chai::chsize_t &RegisterFile::operator[](int n) & {
    assert(n <= Size);
    return registers_[n];
}
chai::chsize_t RegisterFile::getAcc() const { return acc_; }
void RegisterFile::setAcc(chai::chsize_t value) { acc_ = value; }
chai::chsize_t RegisterFile::getPc() const { return pc_; }
void RegisterFile::setPc(chai::chsize_t value) { pc_ = value; }
