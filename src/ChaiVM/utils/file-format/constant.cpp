#include "ChaiVM/utils/file-format/constant.hpp"

namespace chai::utils::fileformat {

void Constant::putType(std::ofstream &ofs) { ofs.put(this->getTag()); }

ConstI64::ConstI64(int64_t data) : data_(data){};

void ConstI64::write(std::ofstream &ofs) {
    ofs.write(reinterpret_cast<const char *>(&data_), sizeof(int64_t));
}

uint8_t ConstI64::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_I64;
}

ConstF64::ConstF64(double data) : data_(data){};

void ConstF64::write(std::ofstream &ofs) {
    ofs.write(reinterpret_cast<const char *>(&data_), sizeof(double));
}

uint8_t ConstF64::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_F64;
}

ConstFuncNameAndType::ConstFuncNameAndType(
    chai::interpreter::Immidiate name_index,
    chai::interpreter::Immidiate descriptor_index)
    : nameIndex_{name_index}, descriptorIndex_{descriptor_index} {};

void ConstFuncNameAndType::write(std::ofstream &ofs) {
    ofs.write(reinterpret_cast<const char *>(&nameIndex_), sizeof(nameIndex_));
    ofs.write(reinterpret_cast<const char *>(&descriptorIndex_),
              sizeof(descriptorIndex_));
}

uint8_t ConstFuncNameAndType::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_FUNC_NAME_AND_TYPE;
}

ConstRawStr::ConstRawStr(std::string str) : len_(str.size()), str_(str) {}

void ConstRawStr::write(std::ofstream &ofs) {
    ofs.write(reinterpret_cast<const char *>(&len_), sizeof(len_));
    ofs << str_;
}

uint8_t ConstRawStr::getTag() {
    return chai::interpreter::CodeManager::ConstantTag::CNST_RAW_STR;
}

} // namespace chai::utils::fileformat
