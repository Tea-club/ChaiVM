#include "ChaiVM/interpreter/decoder.hpp"
#include "ChaiVM/utils/bit-magic.hpp"

using namespace chai::interpreter;

Instruction Decoder::parse(const uint32_t word) const {
    const Opcode opcode = utils::ExtractBits<uint32_t, 8, 0>(word);
    return Instruction{
        .operation = opcodes2operation[opcode],
        .immidiate = utils::ExtractBits<uint32_t, Immidiate, 24, 8>(word),
        .r1 = utils::ExtractBits<uint32_t, RegisterId, 8, 8>(word),
        .r2 = utils::ExtractBits<uint32_t, RegisterId, 8, 16>(word),
    };
}

// @todo #8:90m Implement decode method. It should firstly fetch instruction and
//  then parse it.
Instruction Decoder::decode(const chsize_t pc) {
    return Instruction{Operation::Inv};
}
