#ifndef CHAIVM_DECODER_HPP
#define CHAIVM_DECODER_HPP

#include <cassert>
#include "instruction.hpp"
#include "operations.hpp"

namespace chai::interpreter {

class Decoder {
public:
    Instruction parse(uint32_t word) {
        constexpr uint32_t OPCODE_MASK = 0xFF;
        constexpr uint32_t R1_MASK = 0xFF00;
        constexpr uint32_t R2_MASK = 0xFF0000;
        constexpr uint32_t IMMEDIATE_MASK = 0xFFFFFF00;
        const Opcode opcode = word & OPCODE_MASK;
        return Instruction {
            .operation = opcodes2operation[opcode],
            .immediate = word & IMMEDIATE_MASK,
            .r1 = static_cast<Register>(word & R1_MASK),
            .r2 = static_cast<Register>(word & R2_MASK)
        };
    }
};

}
#endif //CHAIVM_DECODER_HPP
