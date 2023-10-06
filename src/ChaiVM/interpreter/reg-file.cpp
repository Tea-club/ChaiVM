#include "ChaiVM/interpreter/reg-file.hpp"

chai::chsize_t &chai::interpreter::RegisterFIle::operator[](int n) {
    return registers_[n];
}
chai::chsize_t chai::interpreter::RegisterFIle::getAcc() const { return acc_; }
void chai::interpreter::RegisterFIle::setAcc(chai::chsize_t value) {
    acc_ = value;
}
chai::chsize_t chai::interpreter::RegisterFIle::getPc() const { return pc_; }
void chai::interpreter::RegisterFIle::setPc(chai::chsize_t value) {
    acc_ = value;
}
