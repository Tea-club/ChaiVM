#include "executor-test-fixture.hpp"

void ExecutorTest::loadRR(chai::interpreter::Operation op,
                          chai::interpreter::RegisterId reg1,
                          chai::interpreter::RegisterId reg2) {
    chaiFile_.addInstr(chai::utils::instr2Raw(op, reg1, reg2));
}

void ExecutorTest::loadRI(chai::interpreter::Operation op,
                          chai::interpreter::RegisterId reg1,
                          chai::interpreter::Immidiate imm) {
    chaiFile_.addInstr(chai::utils::inst2RawRI(op, reg1, imm));
}

int ExecutorTest::loadI(chai::interpreter::Operation op,
                        chai::interpreter::Immidiate imm) {
    return chaiFile_.addInstr(chai::utils::instr2Raw(op, imm));
}

void ExecutorTest::loadWithConst(chai::interpreter::Operation op,
                                 int64_t data) {
    chaiFile_.addWithConst(op, data);
}

void ExecutorTest::loadWithConst(chai::interpreter::Operation op, double data) {
    chaiFile_.addWithConst(op, data);
}

int ExecutorTest::load(chai::interpreter::Operation op) {
    return chaiFile_.addInstr(chai::utils::instr2Raw(op));
}

void ExecutorTest::update() {
    chaiFile_.toFile(PATH);
    codeManager_.load(PATH);
}

void ExecutorTest::SetUp() {
    PATH = std::string{"test_"}.append(std::string{
        testing::UnitTest::GetInstance()->current_test_info()->name()});
    std::remove(PATH.c_str());
}

void ExecutorTest::TearDown() { std::remove(PATH.c_str()); }

std::atomic<int> ExecutorTest::counter{0};
