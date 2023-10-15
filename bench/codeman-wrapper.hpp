#pragma once

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

using namespace chai::interpreter;
using namespace chai::utils;

class CodeManWrapper {

public:
    void load(Operation op, RegisterId r1, RegisterId r2 = 0) {
        manager_.load(instr2Raw(op, r1, r2));
    }

    void loadi(Operation op, Immidiate imm) {
        manager_.load(instr2Raw(op, imm));
    }

    void load(Operation op) { manager_.load(instr2Raw(op)); }

public:
    CodeManager manager_;
};
