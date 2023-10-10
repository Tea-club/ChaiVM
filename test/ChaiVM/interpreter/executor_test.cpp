#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"

using namespace chai::interpreter;

class ExecutorTest : public ::testing::Test {
protected:
    /**
     * Calculate raw bytecode of {@link Instruction}.
     * @param instr Instruction.
     * @return Bytecode.
     * @todo #8:90min Now this method processes R and RR formats only.
     *  It should also work with I type. We need smth like Instr2Format
     *  for it.
     */
    static chai::bytecode_t Instr2Raw(Instruction instr) {
        return (operation2opcode(instr.operation)) | (instr.r1 << 4) |
               (instr.r2 << 8);
    }

    static chai::bytecode_t Instr2Raw(Operation op, RegisterId r1,
                                      RegisterId r2) {
        return (operation2opcode(op)) | (r1 << 8) | (r2 << 16);
    }

    static chai::bytecode_t Instr2Raw(Operation op, Immidiate imm) {
        return (operation2opcode(op)) | (imm << 8);
    }

    CodeManager codeManager;
    Executor exec{&codeManager};

private:
    /*
     * @todo #8:90min Implement this method clearer. Now it use the trick that
     *  opcodes and enum values of corresponding operations are the same.
     */
    static uint8_t operation2opcode(Operation operation) {
        return (uint8_t)operation;
    }
};

/*
    Ldia 6
    Star r2
    Ldia 8
    Star r3
    Ldra r3
    Mul r2
    Printc acc
    Ret
 */
TEST_F(ExecutorTest, run) {
    codeManager.load(Instr2Raw(Ldia, 6));
    codeManager.load(Instr2Raw(Star, 2, 0));
    codeManager.load(Instr2Raw(Ldia, 8));
    codeManager.load(Instr2Raw(Star, 3, 0));
    codeManager.load(Instr2Raw(Ldra, 3, 0));
    codeManager.load(Instr2Raw(Mul, 2, 0));
    codeManager.load(Instr2Raw(Ret, 0, 0));
    exec.run();

    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 7);
    EXPECT_EQ(exec.getState().acc(), 48);
}

TEST_F(ExecutorTest, inv) {
    Instruction invIns = {Inv, 0, 0, 0};
    EXPECT_THROW(exec.execute(invIns), InvalidInstruction);
}

TEST_F(ExecutorTest, ret) {
    Instruction retIns = {Ret, 0, 0, 0};
    exec.execute(retIns);
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t));
}

/**
 * @todo #8:5m>/DEV add other tests after implemented decoder
 */
