#pragma once

#include "vector"
#include <cstdint>

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"
#include "ChaiVM/utils/io-bytes.hpp"

namespace chai::utils::fileformat {

enum FieldTag : uint8_t { I64 = 1, F64 = 2 };

struct FieldInfo {
    interpreter::Immidiate name_;
    uint8_t isObject_;
    interpreter::Immidiate tagOrKlassNum_;
};

struct KlassInfo {
    interpreter::Immidiate name_;
    uint8_t nFields_;
    std::vector<FieldInfo> fields_;

    chai::interpreter::Immidiate addField(FieldInfo field);

    void dump(std::ofstream &ofs) const;
};

} // namespace chai::utils::fileformat
