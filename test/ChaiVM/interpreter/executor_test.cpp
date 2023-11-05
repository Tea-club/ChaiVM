#include <cmath>
#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/file-format/constant.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

using chai::bytecode_t;
using chai::utils::inst2RawRI;
using chai::utils::instr2Raw;

class ExecutorTest : public ::testing::Test {
protected:
    static constexpr RegisterId R0 = 1;
    static constexpr RegisterId R1 = 1;
    static constexpr RegisterId R2 = 2;
    static constexpr RegisterId R3 = 3;
    static constexpr RegisterId R4 = 4;
    static constexpr RegisterId R5 = 5;
    static constexpr RegisterId R6 = 6;
    static constexpr RegisterId R7 = 7;
    static constexpr RegisterId R8 = 8;
    static constexpr RegisterId R9 = 9;
    static constexpr RegisterId R10 = 10;
    static constexpr RegisterId R11 = 11;

    const std::filesystem::path PATH{"./exec-testing.chai"};

    /*
     * @todo #42:60min Rename all load methods to more appropriate names.
     */
    void loadRR(Operation op, RegisterId reg1, RegisterId reg2 = 0) {
        chaiFile_.addInstr(instr2Raw(op, reg1, reg2));
    }

    void loadRI(Operation op, RegisterId reg1, Immidiate imm) {
        chaiFile_.addInstr(inst2RawRI(op, reg1, imm));
    }

    int loadI(Operation op, Immidiate imm) {
        return chaiFile_.addInstr(instr2Raw(op, imm));
    }

    void loadWithConst(Operation op, int64_t data) {
        chaiFile_.addWithConst(op, data);
    }

    void loadWithConst(Operation op, double data) {
        chaiFile_.addWithConst(op, data);
    }

    int load(Operation op) { return chaiFile_.addInstr(instr2Raw(op)); }

    void update() {
        chaiFile_.toFile(PATH);
        codeManager_.load(PATH);
    }

    void TearDown() override { std::remove(PATH.c_str()); }

    ChaiFile chaiFile_;
    CodeManager codeManager_;
    chai::memory::LinearBuffer buffer_ = chai::memory::LinearBuffer(1024*256);
    Executor exec_{&codeManager_, buffer_};
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
    loadWithConst(Ldia, static_cast<int64_t>(6));
    loadRR(Star, 2, 0);
    loadWithConst(Ldia, static_cast<int64_t>(8));
    loadRR(Star, 3, 0);
    loadRR(Ldra, 3, 0);
    loadRR(Mul, 2, 0);
    loadRR(Ret, 0, 0);
    update();
    exec_.run();

//    EXPECT_EQ(exec_.pc(), sizeof(chai::bytecode_t) * 7);
//    EXPECT_EQ(exec_.acc(), 48);
}
//TEST_F(ExecutorTest, inv) {
//    load(Inv);
//    update();
//    EXPECT_THROW(exec_.run(), InvalidInstruction);
//}
//TEST_F(ExecutorTest, nop) {
//    load(Nop);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.pc(), sizeof(chai::bytecode_t) * 2);
//}
//TEST_F(ExecutorTest, ret) {
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.pc(), sizeof(chai::bytecode_t));
//}

//TEST_F(ExecutorTest, mov) {
//    int64_t val = -100L;
//    RegisterId r1 = 0;
//    RegisterId r2 = 1;
//    loadWithConst(Ldia, val);
//    loadRR(Star, r1, 0);
//    loadRR(Mov, r1, r2);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState()[r1], exec_.getState()[r2]);
//    EXPECT_EQ(static_cast<int>(exec_.getState()[r2]), static_cast<int>(val));
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 4);
//}
//TEST_F(ExecutorTest, ldia) {
//    int64_t val = -1L;
//    loadWithConst(Ldia, val);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(static_cast<int>(exec_.getState().acc()), static_cast<int>(val));
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 2);
//}
//TEST_F(ExecutorTest, ldra) {
//    int64_t val = -3L;
//    RegisterId r1 = 0;
//    loadWithConst(Ldia, val);
//    loadRR(Star, r1, 0);
//    loadRR(Ldra, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState()[r1], exec_.getState().acc());
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()),
//              static_cast<int64_t>(val));
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 4);
//}
//TEST_F(ExecutorTest, star) {
//    int64_t val = 3L;
//    RegisterId r1 = 0;
//    loadWithConst(Ldia, val);
//    loadRR(Star, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), exec_.getState()[r1]);
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState()[r1]), val);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, add) {
//    int64_t val1 = 10;
//    int64_t val2 = 33;
//    loadWithConst(Ldia, val1);
//    loadRR(Star, R1, 0);
//    loadWithConst(Ldia, val2);
//    loadRR(Add, R1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState()[R1], val1);
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val1 + val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, addNeg) {
//    int64_t val1 = -10;
//    int64_t val2 = -33;
//    RegisterId r1 = 0;
//    loadWithConst(Ldia, val1);
//    loadRR(Star, r1, 0);
//    loadWithConst(Ldia, val2);
//    loadRR(Add, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState()[r1], val1);
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val1 + val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, addi) {
//    int64_t val1 = std::numeric_limits<RegisterId>::min();
//    int64_t val2 = std::numeric_limits<RegisterId>::min();
//    loadWithConst(Ldia, val2);
//    loadWithConst(Addi, val1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val1 + val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, sub) {
//    int64_t val1 = 10;
//    int64_t val2 = 33;
//    RegisterId r1 = 0;
//    loadWithConst(Ldia, val1);
//    loadRR(Star, r1, 0);
//    loadWithConst(Ldia, val2);
//    loadRR(Sub, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState()[r1], val1);
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val2 - val1);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, subi) {
//    int64_t val1 = 102;
//    int64_t val2 = -1028;
//    loadWithConst(Ldia, val2);
//    loadWithConst(Subi, val1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val2 - val1);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, mul) {
//    int64_t val1 = -232;
//    int64_t val2 = -33;
//    RegisterId r1 = 0;
//    loadWithConst(Ldia, val1);
//    loadRR(Star, r1, 0);
//    loadWithConst(Ldia, val2);
//    loadRR(Mul, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState()[r1], val1);
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val1 * val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, muli) {
//    int64_t val1 = 124;
//    int64_t val2 = 345;
//    loadWithConst(Ldia, val2);
//    loadWithConst(Muli, val1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val1 * val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, div) {
//    int64_t val1 = 10;
//    int64_t val2 = -33;
//    RegisterId r1 = 1;
//    loadWithConst(Ldia, val1);
//    loadRR(Star, r1, 0);
//    loadWithConst(Ldia, val2);
//    loadRR(Div, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState()[r1], val1);
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val2 / val1);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, divi) {
//    int64_t val1 = 2;
//    int64_t val2 = 4;
//    loadWithConst(Ldia, val2);
//    loadWithConst(Divi, val1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(static_cast<int64_t>(exec_.getState().acc()), val2 / val1);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, ldiaf) {
//    double val = 3.14;
//    loadWithConst(Ldiaf, val);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 2);
//}
//TEST_F(ExecutorTest, addf) {
//    auto val1 = 3.14;
//    auto val2 = 2.71;
//    RegisterId r1 = 0;
//    loadWithConst(Ldiaf, val1);
//    loadRR(Star, r1, 0);
//    loadWithConst(Ldiaf, val2);
//    loadRR(Addf, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState()[r1]), val1);
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val1 + val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, addif) {
//    auto val1 = 3.14;
//    auto val2 = -2.71;
//    loadWithConst(Ldiaf, val2);
//    loadWithConst(Addif, val1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val1 + val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, subf) {
//    auto val1 = 3.14;
//    auto val2 = 2.71;
//    loadWithConst(Ldiaf, val1);
//    loadRR(Star, R0, 0);
//    loadWithConst(Ldiaf, val2);
//    loadRR(Subf, R0, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState()[R0]), val1);
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val2 - val1);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, subif) {
//    auto val1 = 3.14;
//    auto val2 = 2.71;
//    loadWithConst(Ldiaf, val2);
//    loadWithConst(Subif, val1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val2 - val1);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, mulf) {
//    auto val1 = 3.14;
//    auto val2 = 2.71;
//    RegisterId r1 = 0;
//    loadWithConst(Ldiaf, val1);
//    loadRR(Star, r1, 0);
//    loadWithConst(Ldiaf, val2);
//    loadRR(Mulf, r1, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState()[r1]), val1);
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val1 * val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, mulif) {
//    auto val1 = 3.14;
//    auto val2 = 2.71;
//    loadWithConst(Ldiaf, val2);
//    loadWithConst(Mulif, val1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val1 * val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//TEST_F(ExecutorTest, divf) {
//    auto val1 = 3.14;
//    auto val2 = 2.71;
//    loadWithConst(Ldiaf, val2);
//    loadRR(Star, R0, 0);
//    loadWithConst(Ldiaf, val1);
//    loadRR(Divf, R0, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState()[R0]), val2);
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val1 / val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 5);
//}
//TEST_F(ExecutorTest, divif) {
//    auto val1 = 3.14;
//    auto val2 = 2.71;
//    loadWithConst(Ldiaf, val1);
//    loadWithConst(Divif, val2);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), val1 / val2);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(MathTest, icsqrt) {
//    loadWithConst(Ldiaf, 4.0);
//    load(IcSqrt);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(std::bit_cast<double>(exec_.getState().acc()), 2.0);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(MathTest, icsin) {
//    loadWithConst(Ldiaf, 30.0 * static_cast<double>(M_PI) / 180);
//    load(IcSin);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), 0.5);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(MathTest, iccos) {
//    loadWithConst(Ldiaf, 60.0 * (M_PI) / 180);
//    load(IcCos);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), 0.5);
//    EXPECT_EQ(exec_.getState().pc(), sizeof(chai::bytecode_t) * 3);
//}
//
//TEST_F(ExecutorTest, SquareEquation) {
//    // r1 = 1.0, r2 = -5.0, r3 = 6.0
//    loadWithConst(Ldiaf, 1.0);
//    loadRR(Star, R1, 0);
//    loadWithConst(Ldiaf, -5.0);
//    loadRR(Star, R2, 0);
//    loadWithConst(Ldiaf, +6.0);
//    loadRR(Star, R3, 0);
//
//    // r4 = -4*r1*r3
//    loadWithConst(Ldiaf, -4.0);
//    loadRR(Mulf, R1, 0);
//    loadRR(Mulf, R3, 0);
//    loadRR(Star, R4, 0);
//
//    // r5 = b * b
//    loadRR(Ldra, R2, 0);
//    loadRR(Mulf, R2, 0);
//    loadRR(Star, R5, 0);
//
//    // r6 = r5 + r4
//    loadRR(Ldra, R5, 0);
//    loadRR(Addf, R4, 0);
//    loadRR(Star, R6, 0);
//
//    // r6 = sqrt(r6)
//    loadRR(Ldra, R6, 0);
//    load(IcSqrt);
//    loadRR(Star, R6, 0);
//
//    // r7 = 2a
//    loadRR(Ldra, R1, 0);
//    loadWithConst(Mulif, 2.0);
//    loadRR(Star, R7, 0);
//
//    // r8 = r6 - r2
//    loadRR(Ldra, R6, 0);
//    loadRR(Subf, R2, 0);
//    loadRR(Star, R8, 0);
//
//    // X1 = r9 = r8 / r7
//    loadRR(Ldra, R8, 0);
//    loadRR(Divf, R7, 0);
//    loadRR(Star, R9, 0);
//
//    // acc = -r2 - r6
//    // r11 = acc / r7
//    loadWithConst(Ldiaf, 0.0);
//    loadRR(Subf, R2, 0);
//    loadRR(Subf, R6, 0);
//    loadRR(Divf, R7, 0);
//    loadRR(Star, R11, 0);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState()[R9]), 3.0);
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState()[R11]), 2.0);
//    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.getState().acc()), 2.0);
//}
//
//TEST_F(ExecutorTest, If_icmpeq_simple) {
//    Immidiate val = chaiFile_.addConst(std::make_unique<ConstI64>(42));
//    loadI(Ldia, val);
//    loadRR(Star, R1);
//    loadRI(If_icmpeq, R1, 2 * sizeof(bytecode_t));
//    load(Inv);
//    load(Ret);
//    update();
//    exec_.run();
//}
//
//TEST_F(ExecutorTest, If_icmpne_cycle) {
//    constexpr int64_t threshold = 2000;
//    loadWithConst(Ldia, static_cast<int64_t>(1));
//    loadRR(Star, R1);
//    loadWithConst(Ldia, threshold);
//    loadRR(Star, R10);
//    loadWithConst(Ldia, static_cast<int64_t>(0));
//    loadRR(Add, R1);
//    loadRI(If_icmpne, R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), threshold);
//}
//
//TEST_F(ExecutorTest, If_icmpgt_cycle) {
//    constexpr int64_t threshold = -200;
//    loadWithConst(Ldia, static_cast<int64_t>(1));
//    loadRR(Star, R1);
//    loadWithConst(Ldia, threshold);
//    loadRR(Star, R10);
//    loadWithConst(Ldia, static_cast<int64_t>(0));
//    loadRR(Sub, R1);
//    loadRI(If_icmpgt, R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), threshold);
//}
//
//TEST_F(ExecutorTest, If_icmpge_cycle) {
//    constexpr int64_t threshold = -200;
//    loadWithConst(Ldia, static_cast<int64_t>(1));
//    loadRR(Star, R1);
//    loadWithConst(Ldia, threshold);
//    loadRR(Star, R10);
//    loadWithConst(Ldia, static_cast<int64_t>(0));
//    loadRR(Sub, R1);
//    loadRI(If_icmpge, R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), threshold - 1);
//}
//
//TEST_F(ExecutorTest, If_icmplt_cycle) {
//    constexpr int64_t threshold = 200;
//    loadWithConst(Ldia, static_cast<int64_t>(1));
//    loadRR(Star, R1);
//    loadWithConst(Ldia, threshold);
//    loadRR(Star, R10);
//    loadWithConst(Ldia, static_cast<int64_t>(0));
//    loadRR(Add, R1);
//    loadRI(If_icmplt, R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), threshold);
//}
//
//TEST_F(ExecutorTest, If_icmple_cycle) {
//    constexpr int64_t threshold = 200;
//    loadWithConst(Ldia, static_cast<int64_t>(1));
//    loadRR(Star, R1);
//    loadWithConst(Ldia, threshold);
//    loadRR(Star, R10);
//    loadWithConst(Ldia, static_cast<int64_t>(0));
//    loadRR(Add, R1);
//    loadRI(If_icmple, R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), threshold + 1);
//}
//
//TEST_F(ExecutorTest, Cmpgf_greater) {
//    loadWithConst(Ldiaf, 0.1234567);
//    loadRR(Star, R1);
//    loadWithConst(Ldiaf, 123.9);
//    // acc > r1
//    loadRR(Сmpgf, R1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), 1);
//}
//
//TEST_F(ExecutorTest, Cmpgf_less) {
//    loadWithConst(Ldiaf, 0.1234567);
//    loadRR(Star, R1);
//    loadWithConst(Ldiaf, -123.9);
//    // acc < r1
//    loadRR(Сmpgf, R1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), -1);
//}
//
//TEST_F(ExecutorTest, Cmpgf_equal) {
//    loadWithConst(Ldiaf, 0.123456789);
//    loadRR(Star, R1);
//    loadWithConst(Ldiaf, 0.123456789);
//    // acc < r1
//    loadRR(Сmpgf, R1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), 0);
//}
//
//TEST_F(ExecutorTest, Cmpgf_nan) {
//    loadWithConst(Ldiaf, NAN);
//    loadRR(Star, R1);
//    loadWithConst(Ldiaf, 0.123456789);
//    // r1 is NaN
//    loadRR(Сmpgf, R1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), -1);
//}
//
//TEST_F(ExecutorTest, Cmplf_less) {
//    loadWithConst(Ldiaf, 0.15);
//    loadRR(Star, R1);
//    loadWithConst(Ldiaf, -1234.5);
//    // acc < r1
//    loadRR(Cmplf, R1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), 1);
//}
//
//TEST_F(ExecutorTest, Cmplf_nan) {
//    loadWithConst(Ldiaf, 0.15);
//    loadRR(Star, R1);
//    loadWithConst(Ldiaf, NAN);
//    // acc is NaN
//    loadRR(Cmplf, R1);
//    load(Ret);
//    update();
//    exec_.run();
//    EXPECT_EQ(exec_.getState().acc(), -1);
//}
//
//TEST_F(ExecutorTest, Goto_forward) {
//    constexpr int ret = 345;
//    loadI(Goto, (ret) * sizeof(bytecode_t));
//    for (int i = 0; i < ret - 1; ++i) {
//        load(Inv);
//    }
//    ASSERT_EQ(load(Ret), ret);
//    for (int i = 0; i < 500; ++i) {
//        load(Inv);
//    }
//    update();
//    exec_.run();
//}
//
//TEST_F(ExecutorTest, Goto_forward_and_back) {
//    // jump to
//    constexpr int ret1 = 345;
//    constexpr int ret2 = ret1 + 5;
//    loadI(Goto, (ret2) * sizeof(bytecode_t));
//    for (int i = 0; i < ret1 - 1; ++i) {
//        load(Inv);
//    }
//    ASSERT_EQ(load(Ret), ret1);
//    load(Inv);
//    load(Inv);
//    load(Inv);
//    load(Inv);
//    ASSERT_EQ(loadI(Goto, static_cast<Immidiate>(-5 * sizeof(bytecode_t))),
//              ret2);
//    for (int i = 0; i < 500; ++i) {
//        load(Inv);
//    }
//    update();
//    exec_.run();
//}
