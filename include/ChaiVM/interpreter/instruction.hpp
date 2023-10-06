#ifndef CHAIVM_INSTRUCTION_HPP
#define CHAIVM_INSTRUCTION_HPP

#include <cstdint>

namespace chai::interpreter {


using Opcode = uint8_t;
using Register = uint8_t;
using Immediate = uint32_t;

enum Operation {
    Inv,
};

struct Instruction {
    Operation operation;
    Immediate immediate;
    Register src;
    Register dst;
};


}

#endif //CHAIVM_INSTRUCTION_HPP
