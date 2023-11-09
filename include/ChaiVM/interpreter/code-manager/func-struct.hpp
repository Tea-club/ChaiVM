#pragma once

#include <vector>

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

namespace chai::interpreter {

struct Function {
    uint8_t numRegs;
    uint8_t numArgs;
    Immidiate constFuncRef;
    std::vector<bytecode_t> code = {};
};

} // namespace chai::interpreter
