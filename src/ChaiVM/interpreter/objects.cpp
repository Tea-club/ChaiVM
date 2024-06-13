#include <cassert>

#include "ChaiVM/interpreter/frame.hpp"
#include "ChaiVM/interpreter/objects.hpp"

namespace chai::interpreter {

Object::Object(ObjectHeader *header, chsize_t *fields)
    : header_(header), members_(fields) {}

Object::Object(chsize_t ref)
    : header_(std::bit_cast<ObjectHeader *>(ref)),
      members_(std::bit_cast<chsize_t *>(header_ + 1)) {}

chsize_t Object::countMembers() const {
    return (header_->size_ - sizeof(ObjectHeader)) / sizeof(chsize_t);
}

const chsize_t &Object::getMember(Immidiate offset) const {
    assert(offset % sizeof(chsize_t) == 0);
    return members_[offset / sizeof(chsize_t)];
}

void Object::setMember(Immidiate offset, chsize_t value) const {
    assert(offset % sizeof(chsize_t) == 0);
    members_[offset / sizeof(chsize_t)] = value;
}

IndexOutOfBoundary::IndexOutOfBoundary(const char *msg) : runtime_error(msg) {}
IndexOutOfBoundary::IndexOutOfBoundary(const std::string &msg)
    : runtime_error(msg) {}
const char *IndexOutOfBoundary::what() const noexcept {
    return runtime_error::what();
}

ObjectArray::ObjectArray(chsize_t ref) : Object(ref) {}

chsize_t ObjectArray::length() const { return Object::countMembers() - 1; }

chai::chsize_t &ObjectArray::operator[](int64_t i) & {
    assert(i >= 0);
    if (i >= length()) {
        throw IndexOutOfBoundary("index " + std::to_string(i) +
                                 " is greater than array length " +
                                 std::to_string(length()));
    }
    return Object::members_[i + 1];
}

const chsize_t &ObjectArray::operator[](int64_t i) const & {
    assert(i >= 0);
    if (i >= length()) {
        throw IndexOutOfBoundary("index " + std::to_string(i) +
                                 " is greater than array length " +
                                 std::to_string(length()));
    }
    return Object::members_[i + 1];
}

chsize_t ObjectArray::sizeOfObjectArray(int64_t len) {
    return sizeof(ObjectHeader) + (1 + len) * sizeof(chsize_t);
}

} // namespace chai::interpreter
