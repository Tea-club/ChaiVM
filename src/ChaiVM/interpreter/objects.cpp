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

chsize_t Object::getMember(Immidiate offset) const {
    assert(offset % sizeof(chsize_t) == 0);
    return members_[offset / sizeof(chsize_t)];
}

void Object::setMember(Immidiate offset, chsize_t value) const {
    assert(offset % sizeof(chsize_t) == 0);
    members_[offset / sizeof(chsize_t)] = value;
}

} // namespace chai::interpreter
