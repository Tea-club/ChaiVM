#pragma once

#include <string>

#include "ChaiVM/interpreter/instruction.hpp"

class Operation {
public:
    enum Value {
        Inv,
        Nop,
        Ret,
        Mov,
        Ldia,
        Ldra,
        Star,
        Add,
        Addi,
        Sub,
        Subi,
        Mul,
        Muli,
        Div,
        Divi,
        Ldiaf,
        Addf,
        Addif,
        Subf,
        Subif,
        Mulf,
        Mulif,
        Divf,
        Divif,
        IcPrint,
        IcScani,
        IcScanf,
        IcSqrt,
        IcSin,
        IcCos,
    };

    Operation(Value val);
    explicit Operation(const std::string &strOp);

    std::string_view toString() const;

    constexpr operator Value() const { return val_; }
    constexpr auto operator<=>(const Operation &rhs) const = default;
    constexpr auto operator==(const Value &rhs) const { return this->val_ == rhs; }

private:
    constexpr int findString(const std::string &strOp);

    Value val_ = Inv;

    constexpr static std::string_view  toStr[] = {
        "Inv",   "Nop",   "Ret",   "Mov",     "Ldia",    "Ldra",    "Star",
        "Add",   "Addi",  "Sub",   "Subi",    "Mul",     "Muli",    "Div",
        "Divi",  "Ldiaf", "Addf",  "Addif",   "Subf",    "Subif",   "Mulf",
        "Mulif", "Divf",  "Divif", "IcPrint", "IcScani", "IcScanf", "IcSqrt",
        "IcSin", "IcCos", "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv",   "Inv",     "Inv",     "Inv",     "Inv",
        "Inv",   "Inv",   "Inv"};
};

Operation::Operation(Value val) : val_(val) {}
Operation::Operation(const std::string &strOp) {
    int ind = findString(strOp);
    if (ind == -1) {
        throw std::invalid_argument(
            "There is no enum value corresponding to the string:" + strOp);
    }
    val_ = static_cast<Value>(ind);
}

std::string_view Operation::toString() const { return toStr[val_]; }

constexpr int Operation::findString(const std::string &strOp) {
    for (int i = 0; i < sizeof(toStr); i++) {
        if (strOp == toStr[i]) {
            return i;
        }
    }
    return -1;
}
