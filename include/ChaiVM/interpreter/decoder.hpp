#ifndef CHAIVM_DECODER_HPP
#define CHAIVM_DECODER_HPP

#include "ChaiVM/types.hpp"
#include "instruction.hpp"

namespace chai::interpreter {

class Decoder {
public:
    Instruction parse(uint32_t word);
    Instruction decode(chsize_t pc);
};

} // namespace chai::interpreter

#endif // CHAIVM_DECODER_HPP
