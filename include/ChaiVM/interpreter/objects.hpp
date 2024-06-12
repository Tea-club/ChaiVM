#pragma once

#include "ChaiVM/interpreter/code-manager/klass.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include <cassert>

namespace chai::interpreter {

// @todo #97:90min Add allocator here.
struct ObjectHeader {
    chsize_t size_;
    Immidiate klassId_;
};

/**
 * Facade to manage object.
 * Note, it does not own header and fields.
 * @todo #97:30min Consider refactoring object to just cast it from raw pointer.
 *  It can be faster.
 */
class Object {

public:
    explicit Object(ObjectHeader *header, chsize_t *fields);
    /**
     * Ctor.
     * Create object from ref to object.
     * @param ref Ref to object (usually contains in register).
     */
    explicit Object(chsize_t ref);

    chsize_t getField(Immidiate offset) const;

    void setField(Immidiate offset, chsize_t value) const;

private:
    ObjectHeader *header_;
    chsize_t *fields_;
};

} // namespace chai::interpreter
