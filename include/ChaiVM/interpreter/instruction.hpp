#ifndef CHAIVM_INSTRUCTION_HPP
#define CHAIVM_INSTRUCTION_HPP

#include "operations.hpp"
#include <cstdint>

namespace chai::interpreter {

using Opcode = uint8_t;
using RegisterId = uint8_t;
using Immidiate = uint32_t;

struct Instruction {
    Operation operation;
    Immidiate immidiate;
    RegisterId r1;
    RegisterId r2;
};

} // namespace chai::interpreter

#endif // CHAIVM_INSTRUCTION_HPP
