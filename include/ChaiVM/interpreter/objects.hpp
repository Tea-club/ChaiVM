#pragma once

#include "ChaiVM/interpreter/code-manager/klass.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"

namespace chai::interpreter {

// @todo:90min Add allocator here.
struct ObjectHeader {
    chsize_t size_;
    Immidiate klassId_;
};

/**
 * Facade to manage object.
 * Note, it does not own header and fields.
 */
class Object {

public:
    explicit Object(ObjectHeader *header, chsize_t *fields)
        : header_(header), fields_(fields) {}

    /**
     * Ctor.
     * Create object from ref to object.
     * @param ref Ref to object (usually contains in register).
     */
    explicit Object(chsize_t ref) : header_(nullptr), fields_(nullptr) {}

private:
    ObjectHeader *header_;
    chsize_t *fields_;
};

} // namespace chai::interpreter
