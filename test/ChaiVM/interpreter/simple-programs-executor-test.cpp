#include "executor-test-fixture.hpp"

using chai::bytecode_t;
using chai::utils::inst2RawRI;
using chai::utils::instr2Raw;
using namespace chai::interpreter;
using namespace chai::utils::fileformat;

TEST_F(ExecutorTest, SquareEquation) {
    // r1 = 1.0, r2 = -5.0, r3 = 6.0
    loadWithConst(Ldiaf, 1.0);
    loadRR(Star, R1, 0);
    loadWithConst(Ldiaf, -5.0);
    loadRR(Star, R2, 0);
    loadWithConst(Ldiaf, +6.0);
    loadRR(Star, R3, 0);

    // r4 = -4*r1*r3
    loadWithConst(Ldiaf, -4.0);
    loadRR(Mulf, R1, 0);
    loadRR(Mulf, R3, 0);
    loadRR(Star, R4, 0);

    // r5 = b * b
    loadRR(Ldra, R2, 0);
    loadRR(Mulf, R2, 0);
    loadRR(Star, R5, 0);

    // r6 = r5 + r4
    loadRR(Ldra, R5, 0);
    loadRR(Addf, R4, 0);
    loadRR(Star, R6, 0);

    // r6 = sqrt(r6)
    loadRR(Ldra, R6, 0);
    load(IcSqrt);
    loadRR(Star, R6, 0);

    // r7 = 2a
    loadRR(Ldra, R1, 0);
    loadWithConst(Mulif, 2.0);
    loadRR(Star, R7, 0);

    // r8 = r6 - r2
    loadRR(Ldra, R6, 0);
    loadRR(Subf, R2, 0);
    loadRR(Star, R8, 0);

    // X1 = r9 = r8 / r7
    loadRR(Ldra, R8, 0);
    loadRR(Divf, R7, 0);
    loadRR(Star, R9, 0);

    // acc = -r2 - r6
    // r11 = acc / r7
    loadWithConst(Ldiaf, 0.0);
    loadRR(Subf, R2, 0);
    loadRR(Subf, R6, 0);
    loadRR(Divf, R7, 0);
    loadRR(Star, R11, 0);
    load(Ret);
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), 2.0);
}

TEST_F(ExecutorTest, Factorial) {
    int64_t n = static_cast<int64_t>(5);
    Immidiate one = chaiFile_.addConst(std::make_unique<ConstI64>(1));
    loadWithConst(Ldia, n);
    loadRR(Star, 99);
    Immidiate func_ref = chaiFile_.nextFunc();
    EXPECT_EQ(
        chaiFile_.addFunction(
            UINT16_MAX, "factorial", "(I)I",
            std::vector<bytecode_t>{
                instr2Raw(Ldra, 7, 0), // val2
                inst2RawRI(If_icmpne, one,
                           static_cast<Immidiate>(3 * sizeof(bytecode_t))),
                instr2Raw(Ldia, one), instr2Raw(Ret), instr2Raw(Star, 2, 0),
                instr2Raw(Subi, one), instr2Raw(Star, 7, 0),
                instr2Raw(Call, func_ref), instr2Raw(Mul, 2, 0),
                instr2Raw(Ret)},
            1, 8),
        func_ref);
    loadI(Call, func_ref);
    load(Ret);
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), 120);
}
