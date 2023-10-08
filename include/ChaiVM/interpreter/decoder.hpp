#ifndef CHAIVM_DECODER_HPP
#define CHAIVM_DECODER_HPP

#include "ChaiVM/types.hpp"
#include "instruction.hpp"

namespace chai::interpreter {

class Decoder {
public:
    [[nodiscard]] Instruction parse(uint32_t word) const;
    [[nodiscard]] Instruction decode(chsize_t pc) const;
};

} // namespace chai::interpreter

#endif // CHAIVM_DECODER_HPP
