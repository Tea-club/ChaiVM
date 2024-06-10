#pragma once

#include <cstdint>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

namespace chai::utils {

chai::bytecode_t instr2RawRR(interpreter::Operation op,
                             interpreter::RegisterId r1,
                             interpreter::RegisterId r2);

chai::bytecode_t instr2RawI(interpreter::Operation op,
                            interpreter::Immidiate imm);

chai::bytecode_t instr2RawN(interpreter::Operation op);

chai::bytecode_t instr2RawRI(interpreter::Operation op,
                             interpreter::RegisterId r1,
                             interpreter::Immidiate imm);

} // namespace chai::utils
