#include <cmath>
#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"
#include "constant.hpp"
#include "chai-file.hpp"

using namespace chai::interpreter;
using namespace chai::utils;

class ExecutorTest : public ::testing::Test {
protected:
    const std::filesystem::path PATH{"./exec-testing.chai"};

    chai::chsize_t addConst(int64_t data) {
        return chaiFile_.addConst(
            std::make_unique<ConstI64>(data)
        );
    }

    Immidiate addConst(double data) {
        return chaiFile_.addConst(
            std::make_unique<ConstF64>(data)
        );
    }



    void load(Operation op, RegisterId r1, RegisterId r2) {
        chaiFile_.addInstr(instr2Raw(op, r1, r2));
    }

    void loadWithConst(Operation op, int64_t data) {
        chai::chsize_t id = addConst(data);
        chaiFile_.addInstr(
            instr2Raw(op, id)
        );
    }

    void loadWithConst(Operation op, double data) {
        chai::chsize_t id = addConst(data);
        chaiFile_.addInstr(
            instr2Raw(op, id)
        );
    }

    void load(Operation op) {
        chaiFile_.addInstr(instr2Raw(op));
    }

    void update() {
        chaiFile_.toFIle(PATH);
        codeManager_.load(PATH);
    }

    void TearDown() override { std::remove(PATH.c_str()); }

    ChaiFile chaiFile_;
    CodeManager codeManager_;
    Executor exec_{&codeManager_};
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
    loadWithConst(Ldia, 6L);
    load(Star, 2, 0);
    loadWithConst(Ldia, 8L);
    load(Star, 3, 0);
    load(Ldra, 3, 0);
    load(Mul, 2, 0);
    load(Ret, 0, 0);
    update();
    exec_.run();

    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 7);
    EXPECT_EQ(exec_.getState().acc(), 48);
}
TEST_F(ExecutorTest, inv) {
    load(Inv);
    update();
    EXPECT_THROW(exec_.run(), InvalidInstruction);
}
TEST_F(ExecutorTest, nop) {
    load(Nop);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 2);
}
TEST_F(ExecutorTest, ret) {
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t));
}
TEST_F(ExecutorTest, mov) {
    int64_t val = -100L;
    RegisterId r1 = 0;
    RegisterId r2 = 1;
    loadWithConst(Ldia, val);
    load(Star, r1, 0);
    load(Mov, r1, r2);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState()[r1], exec_.getState()[r2]);
    EXPECT_EQ(static_cast<int>(exec_.getState()[r2]), static_cast<int>(val));
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 4);
}
TEST_F(ExecutorTest, ldia) {
    int64_t val = -1L;
    loadWithConst(Ldia, val);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int>(exec_.getState().acc()), static_cast<int>(val));
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 2);
}
TEST_F(ExecutorTest, ldra) {
    int64_t val = -3L;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val);
    load(Star, r1, 0);
    load(Ldra, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState()[r1], exec_.getState().acc());
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), static_cast<int64_t>(val));
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 4);
}
TEST_F(ExecutorTest, star) {
    int64_t val = 3L;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val);
    load(Star, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState().acc(), exec_.getState()[r1]);
    EXPECT_EQ(static_cast<int64_t>(exec_.getState()[r1]), val);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, add) {
    int64_t val1 = 10;
    int64_t val2 = 33;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val1);
    load(Star, r1, 0);
    loadWithConst(Ldia, val2);
    load(Add, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState()[r1], val1);
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val1 + val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, addNeg) {
    int64_t val1 = -10;
    int64_t val2 = -33;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val1);
    load(Star, r1, 0);
    loadWithConst(Ldia, val2);
    load(Add, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState()[r1], val1);
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val1 + val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, addi) {
    int64_t val1 = std::numeric_limits<RegisterId>::min();
    int64_t val2 = std::numeric_limits<RegisterId>::min();
    loadWithConst(Ldia, val2);
    loadWithConst(Addi, val1);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val1 + val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, sub) {
    int64_t val1 = 10;
    int64_t val2 = 33;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val1);
    load(Star, r1, 0);
    loadWithConst(Ldia, val2);
    load(Sub, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState()[r1], val1);
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val2 - val1);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, subi) {
    int64_t val1 = 102;
    int64_t val2 = -1028;
    loadWithConst(Ldia, val2);
    loadWithConst(Subi, val1);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val2 - val1);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, mul) {
    int64_t val1 = -232;
    int64_t val2 = -33;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val1);
    load(Star, r1, 0);
    loadWithConst(Ldia, val2);
    load(Mul, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState()[r1], val1);
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val1 * val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, muli) {
    int64_t val1 = 124;
    int64_t val2 = 345;
    loadWithConst(Ldia, val2);
    loadWithConst(Muli, val1);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val1 * val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, div) {
    int64_t val1 = 10;
    int64_t val2 = -33;
    RegisterId r1 = 1;
    loadWithConst(Ldia, val1);
    load(Star, r1, 0);
    loadWithConst(Ldia, val2);
    load(Div, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(exec_.getState()[r1], val1);
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val2 / val1);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, divi) {
    int64_t val1 = 2;
    int64_t val2 = 4;
    loadWithConst(Ldia, val2);
    loadWithConst(Divi, val1);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
              val2 / val1);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, ldiaf) {
    double val = 3.14;
    loadWithConst(Ldiaf, val);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState().acc()),
        val);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 2);
}
TEST_F(ExecutorTest, addf) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    RegisterId r1 = 0;
    loadWithConst(Ldiaf, val1);
    load(Star, r1, 0);
    loadWithConst(Ldiaf, val2);
    load(Addf, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState()[r1]),
        val1);
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState().acc()),
        val1 + val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, addif) {
    auto val1 = 3.14;
    auto val2 = -2.71;
    loadWithConst(Ldiaf, val2);
    loadWithConst(Addif, val1);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState().acc()),
        val1 + val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, subf) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    RegisterId r = 0;
    loadWithConst(Ldiaf, val1);
    load(Star, r, 0);
    loadWithConst(Ldiaf, val2);
    load(Subf, r, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState()[r]),
        val1);
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState().acc()),
        val2 - val1);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, subif) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    loadWithConst(Ldiaf, val2);
    loadWithConst(Subif, val1);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState().acc()),
        val2 - val1);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, mulf) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    RegisterId r1 = 0;
    loadWithConst(Ldiaf, val1);
    load(Star, r1, 0);
    loadWithConst(Ldiaf, val2);
    load(Mulf, r1, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState()[r1]),
        val1);
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState().acc()),
        val1 * val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
TEST_F(ExecutorTest, mulif) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    loadWithConst(Ldiaf, val2);
    loadWithConst(Mulif, val1);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(
        std::bit_cast<double>(exec_.getState().acc()),
        val1 * val2);
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
}
TEST_F(ExecutorTest, divf) {
    auto val1 = std::bit_cast<Immidiate>(3.14f);
    auto val2 = std::bit_cast<Immidiate>(2.71f);
    RegisterId r = 0;
    load(Ldiaf, val2));
    load(Star, r, 0));
    load(Ldiaf, val1));
    load(Divf, r, 0));
    load(Ret));
    exec_.run();
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec_.getState()[r])),
        std::bit_cast<float>(val2));
    EXPECT_FLOAT_EQ(
        static_cast<float>(std::bit_cast<double>(exec_.getState().acc())),
        std::bit_cast<float>(val1) / std::bit_cast<float>(val2));
    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
}
//TEST_F(ExecutorTest, divif) {
//    auto val1 = std::bit_cast<Immidiate>(3.14f);
//    auto val2 = std::bit_cast<Immidiate>(2.71f);
//    load(Ldiaf, val1));
//    load(Divif, val2));
//    load(Ret));
//    exec_.run();
//    EXPECT_FLOAT_EQ(
//        static_cast<float>(std::bit_cast<double>(exec_.getState().acc())),
//        std::bit_cast<float>(val1) / std::bit_cast<float>(val2));
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(MathTest, icsqrt) {
//    load(Ldiaf, std::bit_cast<Immidiate>(4.0f)));
//    load(IcSqrt));
//    load(Ret));
//    exec_.run();
//    EXPECT_EQ(static_cast<float>(std::bit_cast<double>(exec_.getState().acc())),
//              2.0f);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(MathTest, icsin) {
//    load(
//        Ldiaf, std::bit_cast<Immidiate>(30 * static_cast<float>(M_PI) / 180)));
//    load(IcSin));
//    load(Ret));
//    exec_.run();
//    EXPECT_FLOAT_EQ(
//        static_cast<float>(std::bit_cast<double>(exec_.getState().acc())), 0.5f);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(MathTest, iccos) {
//    load(
//        Ldiaf, std::bit_cast<Immidiate>(60 * static_cast<float>(M_PI) / 180)));
//    load(IcCos));
//    load(Ret));
//    exec_.run();
//    EXPECT_FLOAT_EQ(
//        static_cast<float>(std::bit_cast<double>(exec_.getState().acc())), 0.5f);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(ExecutorTest, SquareEquation) {
//    const RegisterId r1 = 1;
//    const RegisterId r2 = 2;
//    const RegisterId r3 = 3;
//    const RegisterId r4 = 4;
//    const RegisterId r5 = 5;
//    const RegisterId r6 = 6;
//    const RegisterId r7 = 7;
//    const RegisterId r8 = 8;
//    const RegisterId r9 = 9;
//    const RegisterId r10 = 10;
//    const RegisterId r11 = 11;
//    // r1 = 1.0, r2 = -5.0, r3 = 6.0
//    load(Ldiaf, std::bit_cast<Immidiate>(1.0f));
//    load(Star, r1, 0);
//    load(Ldiaf, std::bit_cast<Immidiate>(-5.0f));
//    load(Star, r2, 0);
//    load(Ldiaf, std::bit_cast<Immidiate>(+6.0f));
//    load(Star, r3, 0);
//
//    // r4 = -4*r1*r3
//    load(Ldiaf, std::bit_cast<Immidiate>(-4.0f));
//    load(Mulf, r1, 0);
//    load(Mulf, r3, 0);
//    load(Star, r4, 0);
//
//    // r5 = b * b
//    load(Ldra, r2, 0);
//    load(Mulf, r2, 0);
//    load(Star, r5, 0);
//
//    // r6 = r5 + r4
//    load(Ldra, r5, 0);
//    load(Addf, r4, 0);
//    load(Star, r6, 0);
//
//    // r6 = sqrt(r6)
//    load(Ldra, r6, 0);
//    load(IcSqrt);
//    load(Star, r6, 0);
//
//    // r7 = 2a
//    load(Ldra, r1, 0);
//    load(chai::interpreter::Mulif, std::bit_cast<Immidiate>(2.0f));
//    load(Star, r7);
//
//    // r8 = r6 - r2
//    load(Ldra, r6, 0);
//    load(Subf, r2, 0);
//    load(Star, r8, 0);
//
//    // X1 = r9 = r8 / r7
//    load(Ldra, r8, 0);
//    load(Divf, r7, 0);
//    load(Star, r9, 0);
//
//    // acc = -r2 - r6
//    // r11 = acc / r7
//    load(Ldiaf, std::bit_cast<Immidiate>(0.0f));
//    load(Subf, r2, 0);
//    load(Subf, r6, 0);
//    load(Divf, r7, 0);
//    load(Star, r11, 0);
//    load(Ret);
//    exec_.run();
//    EXPECT_FLOAT_EQ(
//        static_cast<float>(std::bit_cast<double>(exec_.getState()[r9])), 3.0f);
//    EXPECT_FLOAT_EQ(
//        static_cast<float>(std::bit_cast<double>(exec_.getState()[r11])), 2.0f);
//    EXPECT_FLOAT_EQ(
//        static_cast<float>(std::bit_cast<double>(exec_.getState().acc())), 2.0f);
//}
