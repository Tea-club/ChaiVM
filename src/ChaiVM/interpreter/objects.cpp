#include <cassert>

#include "ChaiVM/interpreter/frame.hpp"
#include "ChaiVM/interpreter/objects.hpp"

namespace chai::interpreter {

Object::Object(ObjectHeader *header, chsize_t *fields)
    : header_(header), members_(fields) {}

Object::Object(chsize_t ref)
    : header_(std::bit_cast<ObjectHeader *>(ref)),
      members_(std::bit_cast<chsize_t *>(header_ + 1)) {}

chsize_t Object::getMember(Immidiate offset) const {
    assert(offset % 8 == 0);
    return members_[offset / 8];
}

void Object::setMember(Immidiate offset, chsize_t value) const {
    assert(offset % 8 == 0);
    members_[offset / 8] = value;
}

} // namespace chai::interpreter
