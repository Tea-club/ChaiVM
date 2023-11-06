#pragma once

#include <vector>

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

namespace chai::interpreter {

struct Function {
    uint8_t num_regs;
    uint8_t num_args;
    Immidiate const_func_ref;
    std::vector<bytecode_t> code = {};
};

} // namespace chai::interpreter
