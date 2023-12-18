#include "ChaiVM/utils/instr2Raw.hpp"

using namespace chai::interpreter;

namespace chai::utils {

static uint8_t operation2opcode(Operation operation) {
    return (uint8_t)operation;
}

chai::bytecode_t instr2Raw(Operation op, RegisterId r1, RegisterId r2) {
    return (operation2opcode(op)) | (r1 << 8) | (r2 << 16);
}

chai::bytecode_t instr2Raw(Operation op, Immidiate imm) {
    return (operation2opcode(op)) | (static_cast<chai::bytecode_t>(imm) << 16);
}

chai::bytecode_t instr2Raw(Operation op) { return (operation2opcode(op)); }

chai::bytecode_t inst2RawRI(Operation op, RegisterId r1, Immidiate imm) {
    return (operation2opcode(op)) | (static_cast<chai::bytecode_t>(imm) << 16) |
           (r1 << 8);
}

} // namespace chai::utils
