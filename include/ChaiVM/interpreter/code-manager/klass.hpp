#pragma once

#include <vector>

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include "ChaiVM/types.hpp"

namespace chai::interpreter {

union Data {
    int64_t ch_int_;
    double ch_float_;
    chsize_t ch_ref_;
};

struct Field {
public:
    chai::interpreter::Immidiate name_;
};

struct Klass {
public:
    chai::interpreter::Immidiate name_;
    std::vector<Field> fields_;

    chsize_t size();
};

} // namespace chai::interpreter
