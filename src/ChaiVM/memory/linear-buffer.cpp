#include "ChaiVM/memory/linear-buffer.hpp"

namespace chai::memory {

LinearBuffer::LinearBuffer(size_t sz)
    : buf_(new char[sz]), curr_(buf_), free_(sz) {}
LinearBuffer::~LinearBuffer() { delete[] buf_; }

size_t LinearBuffer::size() const { return curr_ - buf_ + free_; }
size_t LinearBuffer::offset() const { return curr_ - buf_; }
void *LinearBuffer::currentPosition() const { return curr_; }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void *LinearBuffer::do_allocate(size_t n, size_t alignment) {
    void *aligned_pos =
        std::align(alignment, n, reinterpret_cast<void *&>(curr_), free_);
    curr_ = curr_ + n;
    free_ -= n;
    return aligned_pos;
}
#pragma GCC diagnostic pop
void LinearBuffer::do_deallocate(void * /*unused*/, size_t n,
                                 size_t /*unused*/) {
    curr_ -= n;
    free_ += n;
}
bool LinearBuffer::do_is_equal(const memory_resource &other) const noexcept {
    return this == &other;
}

} // namespace chai::memory
