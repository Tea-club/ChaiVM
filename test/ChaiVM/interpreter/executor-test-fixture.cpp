#include "executor-test-fixture.hpp"

using chai::interpreter::Immidiate;
using namespace chai::utils::fileformat;

Immidiate ExecutorTest::loadRR(chai::interpreter::Operation op,
                               chai::interpreter::RegisterId reg1,
                               chai::interpreter::RegisterId reg2) {
    return chaiFile_.addInstr(chai::utils::instr2RawRR(op, reg1, reg2));
}

Immidiate ExecutorTest::loadRI(chai::interpreter::Operation op,
                               chai::interpreter::RegisterId reg1,
                               chai::interpreter::Immidiate imm) {
    return chaiFile_.addInstr(chai::utils::instr2RawRI(op, reg1, imm));
}

Immidiate ExecutorTest::loadI(chai::interpreter::Operation op,
                              chai::interpreter::Immidiate imm) {
    return chaiFile_.addInstr(chai::utils::instr2RawI(op, imm));
}

Immidiate ExecutorTest::loadN(chai::interpreter::Operation op) {
    return chaiFile_.addInstr(chai::utils::instr2RawN(op));
}

void ExecutorTest::loadWithConst(chai::interpreter::Operation op,
                                 int64_t data) {
    chaiFile_.addWithConst(op, data);
}

void ExecutorTest::loadWithConst(chai::interpreter::Operation op, double data) {
    chaiFile_.addWithConst(op, data);
}

void ExecutorTest::update() {
    chaiFile_.toFile(path_);
    codeManager_.load(path_);
}

void ExecutorTest::SetUp() {
    path_ = std::string{"test_"}.append(std::string{
        testing::UnitTest::GetInstance()->current_test_info()->name()});
    std::remove(path_.c_str());
}

void ExecutorTest::TearDown() { std::remove(path_.c_str()); }
