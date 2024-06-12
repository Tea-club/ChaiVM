#include <cassert>

#include "ChaiVM/interpreter/frame.hpp"
#include "ChaiVM/interpreter/objects.hpp"

namespace chai::interpreter {

Object::Object(ObjectHeader *header, chsize_t *fields) : header_(header), fields_(fields) {}

Object::Object(chsize_t ref) : header_(std::bit_cast<ObjectHeader *>(ref)),
    fields_(std::bit_cast<chsize_t *>(header_ + 1)) {}

chsize_t Object::getField(Immidiate offset) const {
    assert(offset % 8 == 0);
    return fields_[offset / 8];
}

void Object::setField(Immidiate offset, chsize_t value) const {
    assert(offset % 8 == 0);
    fields_[offset / 8] = value;
}

} // namespace chai::interpreter
