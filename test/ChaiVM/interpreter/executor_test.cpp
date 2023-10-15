#include <cmath>
#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

using namespace chai::interpreter;
using namespace chai::utils;

class ExecutorTest : public ::testing::Test {
protected:
    /**
     * Calculate raw bytecode of {@link Instruction}.
     * @param instr Instruction.
     * @return Bytecode.
     */

    void load(Operation op, RegisterId r1, RegisterId r2) {
        codeManager.load(instr2Raw(op, r1, r2));
    }

    void load(Operation op, Immidiate imm) {
        codeManager.load(instr2Raw(op, imm));
    }

    void load(Operation op) { codeManager.load(instr2Raw(op)); }

    CodeManager codeManager;
    Executor exec{&codeManager};
};

class MathTest : public ExecutorTest {};

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
              static_cast<int>(val2 - val1));
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
              static_cast<int>(val2 - val1));
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
              static_cast<int>(val2 / val1));
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
              static_cast<int>(val2 / val1));
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
    RegisterId r = 0;
    codeManager.load(instr2Raw(Ldiaf, val1));
    codeManager.load(instr2Raw(Star, r));
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Subf, r));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r])),
        std::bit_cast<float>(val1));
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val2) - std::bit_cast<float>(val1));
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
        std::bit_cast<float>(val2) - std::bit_cast<float>(val1));
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
    RegisterId r = 0;
    codeManager.load(instr2Raw(Ldiaf, val2));
    codeManager.load(instr2Raw(Star, r, 0));
    codeManager.load(instr2Raw(Ldiaf, val1));
    codeManager.load(instr2Raw(Divf, r, 0));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r])),
        std::bit_cast<float>(val2));
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) / std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, divif) {
    Immidiate val1 = std::bit_cast<Immidiate>(3.14f);
    Immidiate val2 = std::bit_cast<Immidiate>(2.71f);
    codeManager.load(instr2Raw(Ldiaf, val1));
    codeManager.load(instr2Raw(Divif, val2));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
        std::bit_cast<float>(val1) / std::bit_cast<float>(val2));
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}

TEST_F(MathTest, icsqrt) {
    codeManager.load(instr2Raw(Ldiaf, std::bit_cast<Immidiate>(4.0f)));
    codeManager.load(instr2Raw(IcSqrt));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_EQ(static_cast<float>(std::bit_cast<double>(exec.getState().acc())),
              2.0f);
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}

TEST_F(MathTest, icsin) {
    codeManager.load(instr2Raw(
        Ldiaf, std::bit_cast<Immidiate>(30 * static_cast<float>(M_PI) / 180)));
    codeManager.load(instr2Raw(IcSin));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())), 0.5f);
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}

TEST_F(MathTest, iccos) {
    codeManager.load(instr2Raw(
        Ldiaf, std::bit_cast<Immidiate>(60 * static_cast<float>(M_PI) / 180)));
    codeManager.load(instr2Raw(IcCos));
    codeManager.load(instr2Raw(Ret));
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())), 0.5f);
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t) * 3);
}

TEST_F(ExecutorTest, SquareEquation) {
    const RegisterId r1 = 1;
    const RegisterId r2 = 2;
    const RegisterId r3 = 3;
    const RegisterId r4 = 4;
    const RegisterId r5 = 5;
    const RegisterId r6 = 6;
    const RegisterId r7 = 7;
    const RegisterId r8 = 8;
    const RegisterId r9 = 9;
    const RegisterId r10 = 10;
    const RegisterId r11 = 11;
    // r1 = 1.0, r2 = -5.0, r3 = 6.0
    load(Ldiaf, std::bit_cast<Immidiate>(1.0f));
    load(Star, r1, 0);
    load(Ldiaf, std::bit_cast<Immidiate>(-5.0f));
    load(Star, r2, 0);
    load(Ldiaf, std::bit_cast<Immidiate>(+6.0f));
    load(Star, r3, 0);

    // r4 = -4*r1*r3
    load(Ldiaf, std::bit_cast<Immidiate>(-4.0f));
    load(Mulf, r1, 0);
    load(Mulf, r3, 0);
    load(Star, r4, 0);

    // r5 = b * b
    load(Ldra, r2, 0);
    load(Mulf, r2, 0);
    load(Star, r5, 0);

    // r6 = r5 + r4
    load(Ldra, r5, 0);
    load(Addf, r4, 0);
    load(Star, r6, 0);

    // r6 = sqrt(r6)
    load(Ldra, r6, 0);
    load(IcSqrt);
    load(Star, r6, 0);

    // r7 = 2a
    load(Ldra, r1, 0);
    load(chai::interpreter::Mulif, std::bit_cast<Immidiate>(2.0f));
    load(Star, r7);

    // r8 = r6 - r2
    load(Ldra, r6, 0);
    load(Subf, r2, 0);
    load(Star, r8, 0);

    // X1 = r9 = r8 / r7
    load(Ldra, r8, 0);
    load(Divf, r7, 0);
    load(Star, r9, 0);

    // acc = -r2 - r6
    // r11 = acc / r7
    load(Ldiaf, std::bit_cast<Immidiate>(0.0f));
    load(Subf, r2, 0);
    load(Subf, r6, 0);
    load(Divf, r7, 0);
    load(Star, r11, 0);
    load(Ret);
    exec.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r9])), 3.0f);
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState()[r11])), 2.0f);
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec.getState().acc())), 2.0f);
}
