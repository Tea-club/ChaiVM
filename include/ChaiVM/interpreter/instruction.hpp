#pragma once

#include <cstdint>

#include "operations.hpp"

namespace chai::interpreter {

using Opcode = uint8_t;
using RegisterId = uint8_t;
using Immidiate = uint16_t;

struct Instruction {
    Operation operation;
    Immidiate immidiate;
    RegisterId r1;
    RegisterId r2;
};

} // namespace chai::interpreter
