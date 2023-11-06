#pragma once

#include "ChaiVM/utils/instr2Raw.hpp"
#include "constant.hpp"

class ChaiFile {
public:
    ChaiFile(std::vector<chai::bytecode_t> &&instrs,
             std::vector<std::unique_ptr<Constant>> &&pool)
        : rawInstrs_(instrs), pool_(std::move(pool)) {
        Immidiate name_index = addConst(std::make_unique<ConstRawStr>("main"));
        Immidiate descriptor_index =
            addConst(std::make_unique<ConstRawStr>("()V"));
        const_func_name_and_type_index =
            addConst(std::make_unique<ConstFuncNameAndType>(name_index,
                                                            descriptor_index));
        // Add main function
    }

    ChaiFile()
        : ChaiFile(std::vector<chai::bytecode_t>{},
                   std::vector<std::unique_ptr<Constant>>{}) {}

    chai::interpreter::Immidiate addInstr(chai::bytecode_t raw) {
        rawInstrs_.push_back(raw);
        return rawInstrs_.size() - 1;
    }

    chai::interpreter::Immidiate
    addConst(std::unique_ptr<Constant> &&constant) {
        pool_.push_back(std::move(constant));
        return pool_.size() - 1;
    }

    void addWithConst(chai::interpreter::Operation op, int64_t data) {
        chai::chsize_t id = addConst(std::make_unique<ConstI64>(data));
        addInstr(chai::utils::instr2Raw(op, id));
    }

    void addWithConst(Operation op, double data) {
        chai::chsize_t id = addConst(std::make_unique<ConstF64>(data));
        addInstr(chai::utils::instr2Raw(op, id));
    }

    void toFile(const std::filesystem::path &path) {
        std::ofstream ofs(path, std::ios::binary | std::ios::out);
        if (ofs.good() && ofs.is_open()) {
            Immidiate constants = pool_.size();
            ofs.write(reinterpret_cast<const char *>(&constants),
                      sizeof(constants));
            for (const std::unique_ptr<Constant> &cnst : pool_) {
                cnst->putType(ofs);
                cnst->write(ofs);
            }
            std::cout << "in toFile constants = " << constants << std::endl;
            Immidiate funcs = functions_.size() + 1;
            ofs.write(reinterpret_cast<const char *>(&funcs), sizeof(funcs));
            dumpMainFunc(ofs);
            ofs.close();
        } else {
            throw std::invalid_argument(std::string{"Invalid path "} +
                                        path.string());
        }
    }

    void dumpMainFunc(std::ofstream &ofs) {
        static_assert(sizeof(Immidiate) == 2);
        Immidiate access_flags = UINT16_MAX;
        ofs.write(reinterpret_cast<const char *>(&access_flags),
                  sizeof(access_flags));
        Immidiate const_ref = const_func_name_and_type_index;
        std::cout << "const_ref in dumpMainFunc = " << const_ref << std::endl;
        ofs.write(reinterpret_cast<const char *>(&const_ref),
                  sizeof(const_ref));
        Immidiate atts_count = 1;
        ofs.write(reinterpret_cast<const char *>(&atts_count),
                  sizeof(atts_count));
        Immidiate att_name_index = UINT16_MAX;
        ofs.write(reinterpret_cast<const char *>(&att_name_index),
                  sizeof(att_name_index));
        uint32_t att_len = 6 + rawInstrs_.size() * sizeof(chai::bytecode_t);
        ofs.write(reinterpret_cast<const char *>(&att_len), sizeof(att_len));
        uint8_t max_registers = 100;
        ofs.write(reinterpret_cast<const char *>(&max_registers),
                  sizeof(max_registers));
        uint8_t nargs = 100;
        ofs.write(reinterpret_cast<const char *>(&nargs), sizeof(nargs));
        uint32_t code_len = rawInstrs_.size() * sizeof(chai::bytecode_t);
        ofs.write(reinterpret_cast<const char *>(&code_len), sizeof(code_len));
        for (const auto &ins : rawInstrs_) {
            ofs.write(reinterpret_cast<const char *>(&ins),
                      sizeof(chai::bytecode_t));
        }
    }

private:
    std::vector<chai::bytecode_t> rawInstrs_;
    std::vector<std::unique_ptr<Constant>> pool_;
    std::vector<Function> functions_;
    Immidiate const_func_name_and_type_index;
};
