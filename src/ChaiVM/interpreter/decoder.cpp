#include "ChaiVM/interpreter/decoder.hpp"

using namespace chai::interpreter;

Instruction Decoder::parse(uint32_t word) {
    return Instruction{Operation::Inv};
}
Instruction Decoder::decode(chsize_t pc) { return Instruction{Operation::Inv}; }
