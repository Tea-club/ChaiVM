#include "ChaiVM/utils/file-format/chai-file.hpp"

namespace chai::utils::fileformat {

ChaiFile::ChaiFile(std::vector<chai::bytecode_t> &&instrs,
                   std::vector<std::unique_ptr<Constant>> &&pool)
    : rawInstrs_(instrs), pool_(std::move(pool)) {
    // Add main function
    chai::interpreter::Immidiate name_index =
        addConst(std::make_unique<ConstRawStr>("main"));
    chai::interpreter::Immidiate descriptor_index =
        addConst(std::make_unique<ConstRawStr>("()V"));
    constFuncNameAndTypeIndex_ = addConst(
        std::make_unique<ConstFuncNameAndType>(name_index, descriptor_index));
    codeAttStr_ = addConst(std::make_unique<ConstRawStr>("Code"));
}

ChaiFile::ChaiFile()
    : ChaiFile(std::vector<chai::bytecode_t>{},
               std::vector<std::unique_ptr<Constant>>{}) {}

chai::interpreter::Immidiate ChaiFile::addInstr(chai::bytecode_t raw) {
    rawInstrs_.push_back(raw);
    return rawInstrs_.size() - 1;
}

chai::interpreter::Immidiate
ChaiFile::addConst(std::unique_ptr<Constant> &&constant) {
    pool_.push_back(std::move(constant));
    return pool_.size() - 1;
}

void ChaiFile::addWithConst(chai::interpreter::Operation op, int64_t data) {
    chai::chsize_t id = addConst(std::make_unique<ConstI64>(data));
    addInstr(chai::utils::instr2Raw(op, id));
}

void ChaiFile::addWithConst(chai::interpreter::Operation op, double data) {
    chai::chsize_t id = addConst(std::make_unique<ConstF64>(data));
    addInstr(chai::utils::instr2Raw(op, id));
}

chai::bytecode_t ChaiFile::getWithConst(chai::interpreter::Operation op,
                                        int64_t data) {
    chai::chsize_t id = addConst(std::make_unique<ConstI64>(data));
    return chai::utils::instr2Raw(op, id);
}

chai::bytecode_t ChaiFile::getWithConst(chai::interpreter::Operation op,
                                        double data) {
    chai::chsize_t id = addConst(std::make_unique<ConstF64>(data));
    return chai::utils::instr2Raw(op, id);
}

chai::interpreter::Immidiate
ChaiFile::addFunction(chai::interpreter::Immidiate access_flags,
                      const std::string &name, const std::string &descriptor,
                      const std::vector<chai::bytecode_t> &instrs,
                      uint8_t num_args, uint8_t max_regs) {
    chai::interpreter::Immidiate name_index =
        addConst(std::make_unique<ConstRawStr>(name));
    chai::interpreter::Immidiate descriptor_index =
        addConst(std::make_unique<ConstRawStr>(descriptor));
    chai::interpreter::Immidiate func_name_and_type_index = addConst(
        std::make_unique<ConstFuncNameAndType>(name_index, descriptor_index));
    uint32_t code_len = instrs.size() * sizeof(chai::bytecode_t);
    functions_.push_back(
        FunctionInfo{.accessFlags_ = access_flags,
                     .nameAndTypeIndex_ = func_name_and_type_index,
                     .attsCount_ = 1, // Code only
                     .attNameIndex_ = codeAttStr_,
                     .attLen_ = 6 + code_len,
                     .maxRegisters_ = max_regs,
                     .nargs_ = num_args,
                     .codeLen_ = code_len,
                     .code_ = instrs});
    return func_name_and_type_index;
}

chai::interpreter::Immidiate ChaiFile::nextFunc() const {
    return pool_.size() + 2;
}

void ChaiFile::toFile(const std::filesystem::path &path) const {
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

void ChaiFile::dumpMainFunc(std::ofstream &ofs) const {
    static_assert(sizeof(chai::interpreter::Immidiate) == 2);
    chai::interpreter::Immidiate access_flags = UINT16_MAX;
    ofs.write(reinterpret_cast<const char *>(&access_flags),
              sizeof(access_flags));
    chai::interpreter::Immidiate const_ref = constFuncNameAndTypeIndex_;
    ofs.write(reinterpret_cast<const char *>(&const_ref), sizeof(const_ref));
    chai::interpreter::Immidiate atts_count = 1;
    ofs.write(reinterpret_cast<const char *>(&atts_count), sizeof(atts_count));
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

} // namespace chai::utils::fileformat
