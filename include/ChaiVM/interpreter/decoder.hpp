#ifndef CHAIVM_DECODER_HPP
#define CHAIVM_DECODER_HPP

#include "instruction.hpp"

namespace chai::interpreter {

class Decoder {

public:
    Instruction parse(uint32_t word);
};

} // namespace chai::interpreter

#endif // CHAIVM_DECODER_HPP
