#include "ChaiVM/interpreter/code-manager/klass.hpp"
#include "ChaiVM/interpreter/objects.hpp"

namespace chai::interpreter {

chsize_t Klass::nFields() const { return fields_.size(); }

chsize_t Klass::instanceSize() const {
    return nFields() * sizeof(chsize_t) + sizeof(ObjectHeader);
}

} // namespace chai::interpreter
