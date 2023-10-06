#ifndef CHAIVM_INSTRUCTION_HPP
#define CHAIVM_INSTRUCTION_HPP

#include <cstdint>
#include "operations.hpp"

namespace chai::interpreter {


using Opcode = uint8_t;
using Register = uint8_t;
using Immediate = uint32_t;

struct Instruction {
    Operation operation;
    Immediate immediate;
    Register r1;
    Register r2;
};


}

#endif //CHAIVM_INSTRUCTION_HPP
