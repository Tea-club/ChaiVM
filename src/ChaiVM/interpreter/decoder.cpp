#include "ChaiVM/interpreter/decoder.hpp"
#include "ChaiVM/utils/bit-magic.hpp"

namespace chai::interpreter::decoder {

Instruction parse(bytecode_t word) {
    const Opcode opcode = utils::ExtractBits<uint32_t, 8, 0>(word);
    return Instruction{
        .operation = opcodes2operation[opcode],
        .immidiate = utils::ExtractBits<uint32_t, Immidiate, 24, 8>(word),
        .r1 = utils::ExtractBits<uint32_t, RegisterId, 8, 8>(word),
        .r2 = utils::ExtractBits<uint32_t, RegisterId, 8, 16>(word),
    };
}

} // namespace chai::interpreter::decoder
