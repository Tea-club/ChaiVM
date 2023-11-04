#pragma once

#include "ChaiVM/interpreter/code-manager/code-manager.hpp"
#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

struct Constant {
    void putType(std::ofstream &ofs) { ofs.put(this->getTag()); }

    virtual void write(std::ofstream &ofs) = 0;
    virtual uint8_t getTag() = 0;
    virtual ~Constant() = default;
};

struct ConstI64 : public Constant {
    int64_t data_;

    ConstI64(int64_t data) : data_(data){};

    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&data_), sizeof(int64_t));
    }
    uint8_t getTag() override {
        return chai::interpreter::CodeManager::ConstantTag::CNST_I64;
    }
    ~ConstI64() override = default;
};

struct ConstF64 : public Constant {
    double data_;
    ConstF64(double data) : data_(data){};
    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&data_), sizeof(double));
    }
    uint8_t getTag() override {
        return chai::interpreter::CodeManager::ConstantTag::CNST_F64;
    }
    ~ConstF64() override = default;
};

struct ConstFuncNameAndType : public Constant {
    chai::interpreter::Immidiate name_index_;
    chai::interpreter::Immidiate descriptor_index_;
    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&name_index_), sizeof(name_index_));
        ofs.write(reinterpret_cast<const char *>(&descriptor_index_), sizeof(descriptor_index_));
    }
    uint8_t getTag() override {
        return chai::interpreter::CodeManager::ConstantTag::CNST_FUNC_NAME_AND_TYPE;
    }
};

struct ConstRawStr : public Constant {
    int16_t len;
    std::string str;
    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&len), sizeof(len));
        ofs << str;
    }
    uint8_t getTag() override {
        return chai::interpreter::CodeManager::ConstantTag::CNST_RAW_STR;
    }
};
