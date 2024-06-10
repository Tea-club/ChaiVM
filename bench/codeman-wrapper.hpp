#pragma once

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

using namespace chai::interpreter;
using namespace chai::utils;

class CodeManWrapper {

public:
    const std::filesystem::path PATH{"./exec-testing.chai"};

    ~CodeManWrapper() { std::remove(PATH.c_str()); }

    void load(Operation op, RegisterId r1, RegisterId r2 = 0) {
        chaiFile_.addInstr(instr2RawRR(op, r1, r2));
    }

    void loadWithConst(Operation op, int64_t data) {
        chaiFile_.addWithConst(op, data);
    }

    void loadWithConst(Operation op, double data) {
        chaiFile_.addWithConst(op, data);
    }

    void load(Operation op) { chaiFile_.addInstr(instr2RawN(op)); }

    void update() {
        chaiFile_.toFile(PATH);
        manager_.load(PATH);
    }

public:
    CodeManager manager_;
    chai::utils::fileformat::ChaiFile chaiFile_;
};
