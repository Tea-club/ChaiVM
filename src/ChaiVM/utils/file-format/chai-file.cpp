#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/io-bytes.hpp"

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

/*
 * @todo #78:90min Check if the constant is already in pool_. Then not add it
 *  again in order to avoid duplication of constant.
 */
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
    return functions_.size();
}

chai::interpreter::Immidiate ChaiFile::nextFunc() const {
    return functions_.size() + 1;
}

void ChaiFile::toFile(const std::filesystem::path &path) const {
    std::ofstream ofs(path, std::ios::binary | std::ios::out);
    if (ofs.good() && ofs.is_open()) {
        chai::interpreter::Immidiate constants_num = pool_.size();
        writeBytes(ofs, &constants_num);
        for (const std::unique_ptr<Constant> &cnst : pool_) {
            cnst->putType(ofs);
            cnst->write(ofs);
        }
        // since main function too.
        chai::interpreter::Immidiate funcs_num = functions_.size() + 1;
        writeBytes(ofs, &funcs_num);
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
    writeBytes(ofs, &access_flags);
    chai::interpreter::Immidiate const_ref = constFuncNameAndTypeIndex_;
    writeBytes(ofs, &const_ref);
    chai::interpreter::Immidiate atts_count = 1;
    writeBytes(ofs, &atts_count);
    chai::interpreter::Immidiate att_name_index = UINT16_MAX;
    writeBytes(ofs, &att_name_index);
    uint32_t att_len = 6 + rawInstrs_.size() * sizeof(chai::bytecode_t);
    writeBytes(ofs, &att_len);
    uint8_t max_registers = 100;
    writeBytes(ofs, &max_registers);
    uint8_t nargs = 100;
    writeBytes(ofs, &nargs);
    uint32_t code_len = rawInstrs_.size() * sizeof(chai::bytecode_t);
    writeBytes(ofs, &code_len);
    for (const auto &ins : rawInstrs_) {
        writeBytes(ofs, &ins);
    }
}

} // namespace chai::utils::fileformat
