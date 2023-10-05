#ifndef CHAIVM_DECODER_HPP
#define CHAIVM_DECODER_HPP

#include "instruction.hpp"

using namespace chai::interpreter;

class Decoder {

public:
    Instruction parse(uint32_t word) ;
};

#endif //CHAIVM_DECODER_HPP
