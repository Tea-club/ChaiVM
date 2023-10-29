#pragma once

#include "../test/ChaiVM/interpreter/chai-file.hpp"
#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

using namespace chai::interpreter;
using namespace chai::utils;

class CodeManWrapper {

public:
    const std::filesystem::path PATH{"./exec-testing.chai"};

    ~CodeManWrapper() { std::remove(PATH.c_str()); }

    chai::chsize_t addConst(int64_t data) {
        return chaiFile_.addConst(std::make_unique<ConstI64>(data));
    }

    Immidiate addConst(double data) {
        return chaiFile_.addConst(std::make_unique<ConstF64>(data));
    }

    void load(Operation op, RegisterId r1, RegisterId r2 = 0) {
        chaiFile_.addInstr(instr2Raw(op, r1, r2));
    }

    void loadWithConst(Operation op, int64_t data) {
        chai::chsize_t id = addConst(data);
        chaiFile_.addInstr(instr2Raw(op, id));
    }

    void loadWithConst(Operation op, double data) {
        chai::chsize_t id = addConst(data);
        chaiFile_.addInstr(instr2Raw(op, id));
    }

    void load(Operation op) { chaiFile_.addInstr(instr2Raw(op)); }

    void update() {
        chaiFile_.toFIle(PATH);
        manager_.load(PATH);
    }

public:
    CodeManager manager_;
    ChaiFile chaiFile_;
};
