#pragma once

#include <cstdint>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

using namespace chai::interpreter;

namespace chai::utils {

chai::bytecode_t instr2Raw(Operation op, RegisterId r1, RegisterId r2);

chai::bytecode_t instr2Raw(Operation op, Immidiate imm);

chai::bytecode_t instr2Raw(Operation op);

chai::bytecode_t inst2RawRI(Operation op, RegisterId r1, Immidiate imm);

} // namespace chai::utils
