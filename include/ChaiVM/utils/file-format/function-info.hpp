#pragma once

struct FunctionInfo {
    chai::interpreter::Immidiate access_flags;
    chai::interpreter::Immidiate name_and_type_index;
    chai::interpreter::Immidiate atts_count;
    chai::interpreter::Immidiate att_name_index;
    uint32_t att_len;
    uint8_t max_registers;
    uint8_t nargs;
    uint32_t code_len;
    std::vector<chai::bytecode_t> code;

    void dump(std::ofstream &ofs) const {
        ofs.write(reinterpret_cast<const char *>(&access_flags),
                  sizeof(access_flags));
        std::cout << "const_ref in dump of function = " << name_and_type_index
                  << std::endl;
        ofs.write(reinterpret_cast<const char *>(&name_and_type_index),
                  sizeof(name_and_type_index));
        ofs.write(reinterpret_cast<const char *>(&atts_count),
                  sizeof(atts_count));
        ofs.write(reinterpret_cast<const char *>(&att_name_index),
                  sizeof(att_name_index));
        ofs.write(reinterpret_cast<const char *>(&att_len), sizeof(att_len));
        ofs.write(reinterpret_cast<const char *>(&max_registers),
                  sizeof(max_registers));
        ofs.write(reinterpret_cast<const char *>(&nargs), sizeof(nargs));
        ofs.write(reinterpret_cast<const char *>(&code_len), sizeof(code_len));
        for (const auto &ins : code) {
            ofs.write(reinterpret_cast<const char *>(&ins),
                      sizeof(chai::bytecode_t));
        }
    }
};
