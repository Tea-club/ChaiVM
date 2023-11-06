#pragma once

#include "ChaiVM/utils/file-format/constant.hpp"
#include "ChaiVM/utils/file-format/function-info.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

class ChaiFile {
public:
    ChaiFile(std::vector<chai::bytecode_t> &&instrs,
             std::vector<std::unique_ptr<Constant>> &&pool)
        : rawInstrs_(instrs), pool_(std::move(pool)) {
        chai::interpreter::Immidiate name_index =
            addConst(std::make_unique<ConstRawStr>("main"));
        chai::interpreter::Immidiate descriptor_index =
            addConst(std::make_unique<ConstRawStr>("()V"));
        constFuncNameAndTypeIndex_ =
            addConst(std::make_unique<ConstFuncNameAndType>(name_index,
                                                            descriptor_index));
        codeAttStr_ = addConst(std::make_unique<ConstRawStr>("Code"));
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

    void addWithConst(chai::interpreter::Operation op, double data) {
        chai::chsize_t id = addConst(std::make_unique<ConstF64>(data));
        addInstr(chai::utils::instr2Raw(op, id));
    }

    /**
     * A more or less convenient way to add a function to the file
     * @param access_flags
     * @param name
     * @param descriptor like in jvm.
     * @param instrs vector of raw instructions
     * @param num_args Number of arguments of the function.
     * @param max_regs Number of registers to be allocated on the frame.
     * @return immidiate - Id of the function (ConstFuncNameAndType).
     */
    chai::interpreter::Immidiate
    addFunction(chai::interpreter::Immidiate access_flags, std::string name,
                std::string descriptor, std::vector<chai::bytecode_t> instrs,
                uint8_t num_args, uint8_t max_regs = 100) {
        chai::interpreter::Immidiate name_index =
            addConst(std::make_unique<ConstRawStr>(name));
        chai::interpreter::Immidiate descriptor_index =
            addConst(std::make_unique<ConstRawStr>(descriptor));
        chai::interpreter::Immidiate func_name_and_type_index =
            addConst(std::make_unique<ConstFuncNameAndType>(name_index,
                                                            descriptor_index));
        uint32_t code_len = instrs.size() * sizeof(chai::bytecode_t);
        functions_.push_back(
            FunctionInfo{.access_flags = access_flags,
                         .name_and_type_index = func_name_and_type_index,
                         .atts_count = 1, // Code only
                         .att_name_index = codeAttStr_,
                         .att_len = 6 + code_len,
                         .max_registers = max_regs,
                         .nargs = num_args,
                         .code_len = code_len,
                         .code = instrs});
        return func_name_and_type_index;
    }

    void toFile(const std::filesystem::path &path) {
        std::ofstream ofs(path, std::ios::binary | std::ios::out);
        if (ofs.good() && ofs.is_open()) {
            chai::interpreter::Immidiate constants = pool_.size();
            ofs.write(reinterpret_cast<const char *>(&constants),
                      sizeof(constants));
            for (const std::unique_ptr<Constant> &cnst : pool_) {
                cnst->putType(ofs);
                cnst->write(ofs);
            }
            // since main function too.
            chai::interpreter::Immidiate funcs = functions_.size() + 1;
            ofs.write(reinterpret_cast<const char *>(&funcs), sizeof(funcs));
            dumpMainFunc(ofs);
            for (const auto &func : functions_) {
                func.dump(ofs);
            }
            ofs.close();
        } else {
            throw std::invalid_argument(std::string{"Invalid path "} +
                                        path.string());
        }
    }

    void dumpMainFunc(std::ofstream &ofs) {
        static_assert(sizeof(chai::interpreter::Immidiate) == 2);
        chai::interpreter::Immidiate access_flags = UINT16_MAX;
        ofs.write(reinterpret_cast<const char *>(&access_flags),
                  sizeof(access_flags));
        chai::interpreter::Immidiate const_ref = constFuncNameAndTypeIndex_;
        ofs.write(reinterpret_cast<const char *>(&const_ref),
                  sizeof(const_ref));
        chai::interpreter::Immidiate atts_count = 1;
        ofs.write(reinterpret_cast<const char *>(&atts_count),
                  sizeof(atts_count));
        chai::interpreter::Immidiate att_name_index = UINT16_MAX;
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

    /**
     * All functions except main.
     */
    std::vector<FunctionInfo> functions_;
    chai::interpreter::Immidiate constFuncNameAndTypeIndex_;
    chai::interpreter::Immidiate codeAttStr_;
};
