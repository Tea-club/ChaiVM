#include "ChaiVM/memory/linear-buffer.hpp"

namespace chai::memory {

LinearBuffer::LinearBuffer(size_t sz) : size_(sz), buf_(new char[size_]) {}
LinearBuffer::LinearBuffer(LinearBuffer &&other) noexcept {
    std::swap(size_, other.size_);
    std::swap(buf_, other.buf_);
}
LinearBuffer &LinearBuffer::operator=(LinearBuffer &&other) noexcept {
    std::swap(size_, other.size_);
    std::swap(buf_, other.buf_);
    return *this;
}

size_t LinearBuffer::size() const { return size_; }
size_t LinearBuffer::offset() const { return offset_; }
void *LinearBuffer::currentPosition() const { return buf_ + offset_; }
void LinearBuffer::shiftOffset(size_t n) { offset_ += n; }

} // namespace chai::memory
