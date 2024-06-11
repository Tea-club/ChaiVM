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

    chai::interpreter::Immidiate addField(FieldInfo field) {
        fields_.push_back(field);
        nFields_++;
        return (nFields_ - 1) * sizeof(chsize_t);
    }

    void dump(std::ofstream &ofs) const {
        writeBytes(ofs, &name_);
        writeBytes(ofs, &nFields_);
        for (const auto &field : fields_) {
            writeBytes(ofs, &field.name_);
            writeBytes(ofs, &field.isObject_);
            writeBytes(ofs, &field.tagOrKlassNum_);
        }
    }
};

} // namespace chai::utils::fileformat
