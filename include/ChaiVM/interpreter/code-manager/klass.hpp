#pragma once

#include <vector>

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include "ChaiVM/types.hpp"

namespace chai::interpreter {

struct Field {
public:
    chai::interpreter::Immidiate name_;
    bool isObject_;
};

struct Klass {
public:
    /**
     * Id of string in constant pool.
     */
    chai::interpreter::Immidiate name_;
    std::vector<Field> fields_;

    chsize_t nFields() const;
    chsize_t instanceSize() const;

    inline bool fieldIsObject(chsize_t offset) const {
        assert(offset % sizeof(chsize_t) == 0);
        return fields_[offset / sizeof(chsize_t)].isObject_;
    }
};

} // namespace chai::interpreter
