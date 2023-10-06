#include "ChaiVM/interpreter/decoder.hpp"

chai::interpreter::Instruction
chai::interpreter::Decoder::parse(uint32_t word) {
    return Instruction{Operation::Inv};
}
chai::interpreter::Instruction
chai::interpreter::Decoder::parse(chai::chsize_t pc) {
    return Instruction{Operation::Inv};
}
