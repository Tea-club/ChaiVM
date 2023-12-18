#pragma once

#include "vector"
#include <cstdint>

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

namespace chai::utils::fileformat {

struct FunctionInfo {
    chai::interpreter::Immidiate accessFlags_;
    chai::interpreter::Immidiate nameAndTypeIndex_;
    chai::interpreter::Immidiate attsCount_;
    chai::interpreter::Immidiate attNameIndex_;
    uint32_t attLen_;
    uint8_t maxRegisters_;
    uint8_t nargs_;
    uint32_t codeLen_;
    std::vector<chai::bytecode_t> code_;

    void dump(std::ofstream &ofs) const;
};

} // namespace chai::utils::fileformat
