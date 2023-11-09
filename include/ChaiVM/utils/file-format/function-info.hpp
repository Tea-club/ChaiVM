#pragma once

struct FunctionInfo {
    chai::interpreter::Immidiate access_flags;
    chai::interpreter::Immidiate name_and_type_index;
    chai::interpreter::Immidiate atts_count;
    chai::interpreter::Immidiate attNameIndex_;
    uint32_t attLen_;
    uint8_t maxRegisters_;
    uint8_t nargs_;
    uint32_t codeLen_;
    std::vector<chai::bytecode_t> code_;

    void dump(std::ofstream &ofs) const;
};
