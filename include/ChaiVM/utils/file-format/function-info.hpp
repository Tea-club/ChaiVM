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

    void dump(std::ofstream &ofs) const {
        ofs.write(reinterpret_cast<const char *>(&access_flags),
                  sizeof(access_flags));
        ofs.write(reinterpret_cast<const char *>(&name_and_type_index),
                  sizeof(name_and_type_index));
        ofs.write(reinterpret_cast<const char *>(&atts_count),
                  sizeof(atts_count));
        ofs.write(reinterpret_cast<const char *>(&attNameIndex_),
                  sizeof(attNameIndex_));
        ofs.write(reinterpret_cast<const char *>(&attLen_), sizeof(attLen_));
        ofs.write(reinterpret_cast<const char *>(&maxRegisters_),
                  sizeof(maxRegisters_));
        ofs.write(reinterpret_cast<const char *>(&nargs_), sizeof(nargs_));
        ofs.write(reinterpret_cast<const char *>(&codeLen_), sizeof(codeLen_));
        for (const auto &ins : code_) {
            ofs.write(reinterpret_cast<const char *>(&ins),
                      sizeof(chai::bytecode_t));
        }
    }
};
