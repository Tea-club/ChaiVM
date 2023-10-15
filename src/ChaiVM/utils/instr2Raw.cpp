#pragma once

#include "ChaiVM/utils/instr2Raw.hpp"

using namespace chai::interpreter;

namespace chai::utils {

chai::bytecode_t instr2Raw(Operation op, RegisterId r1, RegisterId r2) {
    return (operation2opcode(op)) | (r1 << 8) | (r2 << 16);
}

chai::bytecode_t instr2Raw(Operation op, Immidiate imm) {
    return (operation2opcode(op)) | (static_cast<chai::bytecode_t>(imm) << 8);
}

chai::bytecode_t instr2Raw(Operation op) { return (operation2opcode(op)); }

} // namespace chai::utils