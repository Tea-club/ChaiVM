#include "ChaiVM/utils/file-format/constant.hpp"
#include "ChaiVM/utils/io-bytes.hpp"

namespace chai::utils::fileformat {

void Constant::putType(std::ofstream &ofs) { ofs.put(this->getTag()); }

ConstI64::ConstI64(int64_t data) : data_(data){};

void ConstI64::write(std::ofstream &ofs) { writeBytes(ofs, &data_); }

uint8_t ConstI64::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_I64;
}

ConstF64::ConstF64(double data) : data_(data){};

void ConstF64::write(std::ofstream &ofs) { writeBytes(ofs, &data_); }

uint8_t ConstF64::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_F64;
}

ConstFuncNameAndType::ConstFuncNameAndType(
    chai::interpreter::Immidiate name_index,
    chai::interpreter::Immidiate descriptor_index)
    : nameIndex_{name_index}, descriptorIndex_{descriptor_index} {};

void ConstFuncNameAndType::write(std::ofstream &ofs) {
    writeBytes(ofs, &nameIndex_);
    writeBytes(ofs, &descriptorIndex_);
}

uint8_t ConstFuncNameAndType::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_FUNC_NAME_AND_TYPE;
}

ConstRawStr::ConstRawStr(std::string str) : len_(str.size()), str_(str) {}

void ConstRawStr::write(std::ofstream &ofs) {
    writeBytes(ofs, &len_);
    ofs << str_;
}

uint8_t ConstRawStr::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_RAW_STR;
}

} // namespace chai::utils::fileformat
