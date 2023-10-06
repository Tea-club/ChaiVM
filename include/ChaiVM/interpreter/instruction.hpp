#ifndef CHAIVM_INSTRUCTION_HPP
#define CHAIVM_INSTRUCTION_HPP

#include <cstdint>
#include "operations.hpp"

namespace chai::interpreter {

using Opcode = uint8_t;
using RegisterId = uint8_t;
using Immediate = uint32_t;

struct Instruction {
    Operation operation;
    Immediate immediate;
    RegisterId r1;
    RegisterId r2;
};

} // namespace chai::interpreter

#endif // CHAIVM_INSTRUCTION_HPP
