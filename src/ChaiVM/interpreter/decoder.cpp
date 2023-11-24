#include "ChaiVM/interpreter/decoder.hpp"
#include "ChaiVM/utils/bit-magic.hpp"
#include <cassert>

namespace chai::interpreter::decoder {

Instruction parse(bytecode_t word) {
    const Opcode opcode = utils::ExtractBits<bytecode_t, 8, 0>(word);
    assert(opcode <= NewF64Array);
    return Instruction{
        .operation = static_cast<Operation>(opcode),
        .immidiate = utils::ExtractBits<bytecode_t, Immidiate, 16, 16>(word),
        .r1 = utils::ExtractBits<bytecode_t, RegisterId, 8, 8>(word),
        .r2 = utils::ExtractBits<bytecode_t, RegisterId, 8, 16>(word),
    };
}

} // namespace chai::interpreter::decoder
