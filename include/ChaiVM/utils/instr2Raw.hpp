#pragma once

#include <cstdint>

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/interpreter/operations.hpp"
#include "ChaiVM/types.hpp"

using namespace chai::interpreter;

namespace chai::utils {

static uint8_t operation2opcode(Operation operation) {
    return (uint8_t)operation;
}

chai::bytecode_t instr2Raw(Operation op, RegisterId r1, RegisterId r2);

chai::bytecode_t instr2Raw(Operation op, Immidiate imm);

chai::bytecode_t instr2Raw(Operation op);

} // namespace chai::utils