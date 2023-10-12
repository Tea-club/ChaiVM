#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"
#include <unistd.h>

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
    static chai::bytecode_t instr2Raw(Instruction instr) {
        return (operation2opcode(instr.operation)) | (instr.r1 << 8) |
               (instr.r2 << 16);
    }

    static chai::bytecode_t instr2Raw(Operation op, RegisterId r1,
                                      RegisterId r2) {
        return (operation2opcode(op)) | (r1 << 8) | (r2 << 16);
    }
    static chai::bytecode_t instr2Raw(Operation op, Immidiate imm) {
        return (operation2opcode(op)) |
               (static_cast<chai::bytecode_t>(imm) << 8);
    }
    static chai::bytecode_t instr2Raw(Operation op) {
        return (operation2opcode(op));
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
    codeManager.load(instr2Raw(Ldia, 6));
    codeManager.load(instr2Raw(Star, 2, 0));
    codeManager.load(instr2Raw(Ldia, 8));
    codeManager.load(instr2Raw(Star, 3, 0));
    codeManager.load(instr2Raw(Ldra, 3, 0));
    codeManager.load(instr2Raw(Mul, 2, 0));
    codeManager.load(instr2Raw(Ret, 0, 0));
    exec.run();

    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 7);
    EXPECT_EQ(exec.getState().acc(), 48);
}
TEST_F(ExecutorTest, inv) {
    codeManager.load(instr2Raw(Inv));
    EXPECT_THROW(exec.run(), InvalidInstruction);
}
TEST_F(ExecutorTest, nop) {
    codeManager.load(instr2Raw(Nop));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 2);
}
TEST_F(ExecutorTest, ret) {
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t));
}
TEST_F(ExecutorTest, mov) {
    Immidiate val = -100;
    RegisterId r1 = 0;
    RegisterId r2 = 1;
    codeManager.load(instr2Raw(Ldia, val));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Mov, r1, r2));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState()[r1], exec.getState()[r2]);
    EXPECT_EQ(static_cast<int>(exec.getState()[r2]), static_cast<int>(val));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 4);
}
TEST_F(ExecutorTest, ldia) {
    Immidiate val = -1;
    codeManager.load(instr2Raw(Ldia, val));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(static_cast<int>(exec.getState().acc()), static_cast<int>(val));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 2);
}
TEST_F(ExecutorTest, ldra) {
    Immidiate val = -3;
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldia, val));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldra, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState()[r1], exec.getState().acc());
    EXPECT_EQ(static_cast<int>(exec.getState().acc()), static_cast<int>(val));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 4);
}
TEST_F(ExecutorTest, star) {
    Immidiate val = 3;
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldia, val));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState().acc(), exec.getState()[r1]);
    EXPECT_EQ(static_cast<int>(exec.getState()[r1]), static_cast<int>(val));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, add) {
    Immidiate val1 = 10;
    Immidiate val2 = 33;
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldia, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Add, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState()[r1], val1);
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 + val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, addNeg) {
    Immidiate val1 = -10;
    Immidiate val2 = -33;
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldia, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Add, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState()[r1], val1);
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 + val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, addi) {
    Immidiate val1 = std::numeric_limits<RegisterId>::min();
    Immidiate val2 = std::numeric_limits<RegisterId>::min();
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Addi, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 + val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, sub) {
    Immidiate val1 = 10;
    Immidiate val2 = 33;
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldia, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Sub, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState()[r1], val1);
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 - val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, subi) {
    Immidiate val1 = 102;
    Immidiate val2 = -1028;
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Subi, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 - val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, mul) {
    Immidiate val1 = -232;
    Immidiate val2 = -33;
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldia, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Mul, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState()[r1], val1);
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 * val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, muli) {
    Immidiate val1 = 124;
    Immidiate val2 = 345;
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Muli, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 * val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, div) {
    Immidiate val1 = 10;
    Immidiate val2 = -33;
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldia, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Div, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState()[r1], val1);
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 / val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, divi) {
    Immidiate val1 = 2;
    Immidiate val2 = 4;
    codeManager.load(instr2Raw(Ldia, val2));
    codeManager.load(instr2Raw(Divi, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(static_cast<int>(exec.getState().acc()),
              static_cast<int>(val1 / val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, ldiaf) {
    Immidiate val = std::bit_cast<Immidiate>(3.14f);
    codeManager.load(instr2Raw(Ldiaf, val));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 2);
}
TEST_F(ExecutorTest, addf) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldiaf, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Addf, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r1])),
        std::bit_cast<float>(val1));
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) + std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, addif) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(-2.71f);
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Addif, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) + std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, subf) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldiaf, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Subf, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r1])),
        std::bit_cast<float>(val1));
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) - std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, subif) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Subif, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) - std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, mulf) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldiaf, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Mulf, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r1])),
        std::bit_cast<float>(val1));
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) * std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, mulif) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Mulif, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) * std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, divf) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    RegisterId r1 = 0;
    codeManager.load(instr2Raw(Ldiaf, val1));
    codeManager.load(instr2Raw(Star, r1));
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Divf, r1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r1])),
        std::bit_cast<float>(val1));
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) / std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, divif) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Divif, val1));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) / std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}

TEST_F(ExecutorTest, icprintc) {
    codeManager.load(instr2Raw(Ldia, '0'));
    codeManager.load(instr2Raw(IcPrint));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}

TEST_F(ExecutorTest, icscani) {
    constexpr int ERROR = -1;
    int buf_stdin = dup(STDIN_FILENO);
    int mypipe[2];
    EXPECT_EQ(pipe(mypipe), 0);
    write(mypipe[1], "123A", 4);
    char arr[2] = {0};
    EXPECT_NE(dup2(mypipe[0], STDIN_FILENO), ERROR);
    codeManager.load(instr2Raw(IcScani));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(exec.getState().acc(), 123);
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 2);
    EXPECT_NE(dup2(buf_stdin, STDIN_FILENO), ERROR);
    char c;
    scanf("%c", &c);
    EXPECT_EQ(c, 'A');
}
