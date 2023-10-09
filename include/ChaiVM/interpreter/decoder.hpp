#pragma once

#include "ChaiVM/types.hpp"
#include "instruction.hpp"

namespace chai::interpreter::decoder {

Instruction parse(bytecode_t bytecode);

} // namespace chai::interpreter::decoder
