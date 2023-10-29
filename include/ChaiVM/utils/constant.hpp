#pragma once

#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/interpreter/code-manager.hpp"
#include "ChaiVM/types.hpp"

struct Constant {
    void putType(std::ofstream &ofs) { ofs.put(this->getType()); }

    virtual void write(std::ofstream &ofs) = 0;
    virtual int8_t getType() = 0;
    virtual ~Constant() = default;
    ;
};

struct ConstI64 : public Constant {
    int64_t data_;

    ConstI64(int64_t data) : data_(data){};

    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&data_), sizeof(int64_t));
    }
    int8_t getType() override { return chai::interpreter::CodeManager::CNST_I64; }
    ~ConstI64() override = default;
};

struct ConstF64 : public Constant {
    double data_;
    ConstF64(double data) : data_(data){};
    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&data_), sizeof(double));
    }
    int8_t getType() override { return chai::interpreter::CodeManager::CNST_F64; }
    ~ConstF64() override = default;
};
