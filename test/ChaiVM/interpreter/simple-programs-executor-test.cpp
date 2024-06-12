#include "executor-test-fixture.hpp"

using chai::bytecode_t;
using chai::utils::instr2Raw;
using chai::utils::instr2RawI;
using chai::utils::instr2RawN;
using chai::utils::instr2RawRI;
using chai::utils::instr2RawRR;
using namespace chai::interpreter;
using namespace chai::utils::fileformat;

TEST_F(ExecutorTest, SquareEquation) {
    // r1 = 1.0, r2 = -5.0, r3 = 6.0
    loadWithConst(Ldiaf, 1.0);
    load<Star>(R1);
    loadWithConst(Ldiaf, -5.0);
    load<Star>(R2);
    loadWithConst(Ldiaf, +6.0);
    load<Star>(R3);

    // r4 = -4*r1*r3
    loadWithConst(Ldiaf, -4.0);
    load<Mulf>(R1);
    load<Mulf>(R3);
    load<Star>(R4);

    // r5 = b * b
    load<Ldra>(R2);
    load<Mulf>(R2);
    load<Star>(R5);

    // r6 = r5 + r4
    load<Ldra>(R5);
    load<Addf>(R4);
    load<Star>(R6);

    // r6 = sqrt(r6)
    load<Ldra>(R6);
    load<IcSqrt>();
    load<Star>(R6);

    // r7 = 2a
    load<Ldra>(R1);
    loadWithConst(Mulif, 2.0);
    load<Star>(R7);

    // r8 = r6 - r2
    load<Ldra>(R6);
    load<Subf>(R2);
    load<Star>(R8);

    // X1 = r9 = r8 / r7
    load<Ldra>(R8);
    load<Divf>(R7);
    load<Star>(R9);

    // acc = -r2 - r6
    // r11 = acc / r7
    loadWithConst(Ldiaf, 0.0);
    load<Subf>(R2);
    load<Subf>(R6);
    load<Divf>(R7);
    load<Star>(R11);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), 2.0);
}

/*
 * factorial(i):
 *   return i != 1 ?
 *     factorial(i-1)
 *     :
 *     1
 */
TEST_F(ExecutorTest, Factorial) {
    auto n = static_cast<int64_t>(5);
    Immidiate one = chaiFile_.addConst(std::make_unique<ConstI64>(1));
    loadWithConst(Ldia, n);
    load<Star>(99);
    Immidiate func_ref = chaiFile_.nextFunc();
    EXPECT_EQ(
        chaiFile_.addFunction(
            UINT16_MAX, "factorial", "(I)I",
            std::vector<bytecode_t>{
                instr2Raw<Ldra>(7), // val2
                instr2Raw<If_icmpne>(
                    one, static_cast<Immidiate>(3 * sizeof(bytecode_t))),
                instr2Raw<Ldia>(one), instr2Raw<Ret>(), instr2Raw<Star>(2),
                instr2Raw<Subi>(one), instr2Raw<Star>(7),
                instr2Raw<Call>(func_ref), instr2Raw<Mul>(2), instr2Raw<Ret>()},
            1, 8),
        func_ref);
    load<Call>(func_ref);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), 120);
}

TEST_F(ExecutorTest, ArraysDemo) {
    constexpr auto i = static_cast<int64_t>(23);
    constexpr auto size = static_cast<int64_t>(50);
    constexpr auto value = static_cast<int64_t>(12345);
    loadWithConst(Ldia, i);
    load<Star>(R3);
    loadWithConst(Ldia, value);
    load<Star>(R4);
    loadWithConst(Ldia, size);
    load<NewI64Array>();
    load<SetI64InArr>(R3, R4);
    load<GetI64FromArr>(R3);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), value);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

/*
 * let str1 = "Hello World"
 * let str2 = "dungeons and dragons"
 * let substr1 = str1.substr(0, 5) //Hello
 * let substr2 = str2.substr(13, 20) //dragons
 * concat(substr1, substr2)
 */
TEST_F(ExecutorTest, StringDemo) {
    Immidiate str1 =
        chaiFile_.addConst(std::make_unique<ConstRawStr>("Hello World"));
    Immidiate str2 = chaiFile_.addConst(
        std::make_unique<ConstRawStr>("dungeons and dragons"));
    loadWithConst(Ldia, static_cast<int64_t>(0));
    load<Star>(R1); // R1 = 0
    loadWithConst(Ldia, static_cast<int64_t>(5));
    load<Star>(R2); // R2 = 5
    loadWithConst(Ldia, static_cast<int64_t>(13));
    load<Star>(R3); // R3 = 13
    loadWithConst(Ldia, static_cast<int64_t>(20));
    load<Star>(R4); // R4 = 20
    load<Ldia>(str1);
    load<StringSlice>(R1, R2);
    load<StringPrint>();
    load<Star>(R10); // R10 = substr1 = "Hello"
    load<Ldia>(str2);
    load<StringSlice>(R3, R4);
    load<Star>(R11); // R11 = substr2 = "dragons"
    load<Ldra>(R10);
    load<StringConcat>(R11);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(codeManager_.getStringByStringPoolPos(exec_.acc()),
              "Hellodragons");
}
