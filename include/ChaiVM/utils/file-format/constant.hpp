#pragma once

#include "ChaiVM/interpreter/code-manager/code-manager.hpp"
#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

namespace chai::utils::fileformat {

struct Constant {
    void putType(std::ofstream &ofs);

    virtual void write(std::ofstream &ofs) = 0;

    virtual uint8_t getTag() = 0;

    virtual ~Constant() = default;
};

struct ConstI64 : public Constant {
    int64_t data_;

    ConstI64(int64_t data);

    void write(std::ofstream &ofs) override;

    uint8_t getTag() override;

    ~ConstI64() override = default;
};

struct ConstF64 : public Constant {
    double data_;

    ConstF64(double data);

    void write(std::ofstream &ofs) override;

    uint8_t getTag() override;

    ~ConstF64() override = default;
};

struct ConstFuncNameAndType : public Constant {
    chai::interpreter::Immidiate nameIndex_;
    chai::interpreter::Immidiate descriptorIndex_;

    ConstFuncNameAndType(chai::interpreter::Immidiate name_index,
                         chai::interpreter::Immidiate descriptor_index);

    void write(std::ofstream &ofs) override;

    uint8_t getTag() override;
};

struct ConstRawStr : public Constant {
    uint16_t len_;
    std::string str_;

    ConstRawStr(std::string str);

    void write(std::ofstream &ofs) override;

    uint8_t getTag() override;
};

} // namespace chai::utils::fileformat