#pragma once

#include "ChaiVM/types.hpp"
#include "ChaiVM/interpreter/instruction.hpp"

struct Constant {
    void putType (std::ofstream& ofs) {
        ofs.put(this->getType());
    }

    virtual void write(std::ofstream& ofs) = 0;
    virtual int8_t getType() = 0;
    virtual ~Constant() = 0;
};

struct ConstI64 : public Constant {
    int64_t data;
    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&data), sizeof(int64_t));
    }
    int8_t getType() override {
        return CodeManager::CNST_I64;
    }
    ~ConstI64() override = default;
};

struct ConstF64 : public Constant {
    double data;
    void write(std::ofstream &ofs) override {
        ofs.write(reinterpret_cast<const char *>(&data), sizeof(double));
    }
    int8_t getType() override {
        return CodeManager::CNST_F64;
    }
    ~ConstF64() override = default;
};
