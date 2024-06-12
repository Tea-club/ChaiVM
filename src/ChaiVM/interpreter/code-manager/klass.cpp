#include "ChaiVM/interpreter/code-manager/klass.hpp"

namespace chai::interpreter {

chsize_t Klass::size() { return fields_.size(); }

} // namespace chai::interpreter
