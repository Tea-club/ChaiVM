#include "executor-test-fixture.hpp"

using chai::bytecode_t;
using chai::utils::instr2Raw;
using namespace chai::interpreter;
using namespace chai::utils::fileformat;

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
    load<Star>(2);
    loadWithConst(Ldia, static_cast<int64_t>(8));
    load<Star>(3);
    load<Ldra>(3);
    load<Mul>(2);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), 48);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
TEST_F(ExecutorTest, inv) {
    load<Inv>();
    update();
    EXPECT_THROW(exec_.run(), InvalidInstruction);
}
TEST_F(ExecutorTest, nop) {
    load<Nop>();
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
TEST_F(ExecutorTest, ret) {
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(ExecutorTest, mov) {
    int64_t val = -100L;
    RegisterId r1 = 0;
    RegisterId r2 = 1;
    loadWithConst(Ldia, val);
    load<Star>(r1);
    load<Mov>(r1, r2);
    loadWithConst(Mul, static_cast<int64_t>(-1));
    load<Ldra>(r2);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), val);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
TEST_F(ExecutorTest, ldia) {
    int64_t val = -1L;
    loadWithConst(Ldia, val);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int>(exec_.acc()), static_cast<int>(val));
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
TEST_F(ExecutorTest, ldra) {
    int64_t val = -3L;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val);
    load<Star>(r1);
    load<Ldra>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), static_cast<int64_t>(val));
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
TEST_F(ExecutorTest, star) {
    int64_t val = 3L;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val);
    load<Star>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val);
}
TEST_F(ExecutorTest, add) {
    int64_t val1 = 10;
    int64_t val2 = 33;
    loadWithConst(Ldia, val1);
    load<Star>(R1);
    loadWithConst(Ldia, val2);
    load<Add>(R1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val1 + val2);
}
TEST_F(ExecutorTest, addNeg) {
    int64_t val1 = -10;
    int64_t val2 = -33;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val1);
    load<Star>(r1);
    loadWithConst(Ldia, val2);
    load<Add>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val1 + val2);
}
TEST_F(ExecutorTest, addi) {
    int64_t val1 = std::numeric_limits<RegisterId>::min();
    int64_t val2 = std::numeric_limits<RegisterId>::min();
    loadWithConst(Ldia, val2);
    loadWithConst(Addi, val1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val1 + val2);
}
TEST_F(ExecutorTest, sub) {
    int64_t val1 = 10;
    int64_t val2 = 33;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val1);
    load<Star>(r1);
    loadWithConst(Ldia, val2);
    load<Sub>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val2 - val1);
}
TEST_F(ExecutorTest, subi) {
    int64_t val1 = 102;
    int64_t val2 = -1028;
    loadWithConst(Ldia, val2);
    loadWithConst(Subi, val1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val2 - val1);
}
TEST_F(ExecutorTest, mul) {
    int64_t val1 = -232;
    int64_t val2 = -33;
    RegisterId r1 = 0;
    loadWithConst(Ldia, val1);
    load<Star>(r1);
    loadWithConst(Ldia, val2);
    load<Mul>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val1 * val2);
}
TEST_F(ExecutorTest, muli) {
    int64_t val1 = 124;
    int64_t val2 = 345;
    loadWithConst(Ldia, val2);
    loadWithConst(Muli, val1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val1 * val2);
}
TEST_F(ExecutorTest, div) {
    int64_t val1 = 10;
    int64_t val2 = -33;
    RegisterId r1 = 1;
    loadWithConst(Ldia, val1);
    load<Star>(r1);
    loadWithConst(Ldia, val2);
    load<Div>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val2 / val1);
}
TEST_F(ExecutorTest, divi) {
    int64_t val1 = 2;
    int64_t val2 = 4;
    loadWithConst(Ldia, val2);
    loadWithConst(Divi, val1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val2 / val1);
}
TEST_F(ExecutorTest, ldiaf) {
    double val = 3.14;
    loadWithConst(Ldiaf, val);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val);
}
TEST_F(ExecutorTest, addf) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    RegisterId r1 = 0;
    loadWithConst(Ldiaf, val1);
    load<Star>(r1);
    loadWithConst(Ldiaf, val2);
    load<Addf>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val1 + val2);
}
TEST_F(ExecutorTest, addif) {
    auto val1 = 3.14;
    auto val2 = -2.71;
    loadWithConst(Ldiaf, val2);
    loadWithConst(Addif, val1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val1 + val2);
}
TEST_F(ExecutorTest, subf) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    loadWithConst(Ldiaf, val1);
    load<Star>(R0);
    loadWithConst(Ldiaf, val2);
    load<Subf>(R0);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val2 - val1);
}
TEST_F(ExecutorTest, subif) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    loadWithConst(Ldiaf, val2);
    loadWithConst(Subif, val1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val2 - val1);
}
TEST_F(ExecutorTest, mulf) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    RegisterId r1 = 0;
    loadWithConst(Ldiaf, val1);
    load<Star>(r1);
    loadWithConst(Ldiaf, val2);
    load<Mulf>(r1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val1 * val2);
}
TEST_F(ExecutorTest, mulif) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    loadWithConst(Ldiaf, val2);
    loadWithConst(Mulif, val1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val1 * val2);
}
TEST_F(ExecutorTest, divf) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    loadWithConst(Ldiaf, val2);
    load<Star>(R0);
    loadWithConst(Ldiaf, val1);
    load<Divf>(R0);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val1 / val2);
}
TEST_F(ExecutorTest, divif) {
    auto val1 = 3.14;
    auto val2 = 2.71;
    loadWithConst(Ldiaf, val1);
    loadWithConst(Divif, val2);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), val1 / val2);
}

TEST_F(MathTest, icsqrt) {
    loadWithConst(Ldiaf, 4.0);
    load<IcSqrt>();
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(std::bit_cast<double>(exec_.acc()), 2.0);
}

TEST_F(MathTest, icsin) {
    loadWithConst(Ldiaf, 30.0 * static_cast<double>(M_PI) / 180);
    load<IcSin>();
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), 0.5);
}

TEST_F(MathTest, iccos) {
    loadWithConst(Ldiaf, 60.0 * (M_PI) / 180);
    load<IcCos>();
    load<Ret>();
    update();
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), 0.5);
}

TEST_F(ExecutorTest, If_icmpeq_simple) {
    Immidiate val = chaiFile_.addConst(std::make_unique<ConstI64>(42));
    load<Ldia>(val);
    load<Star>(R1);
    load<If_icmpeq>(R1, 2 * sizeof(bytecode_t));
    load<Inv>();
    load<Ret>();
    update();
    exec_.run();
}

TEST_F(ExecutorTest, If_icmpne_cycle) {
    constexpr int64_t threshold = 2000;
    loadWithConst(Ldia, static_cast<int64_t>(1));
    load<Star>(R1);
    loadWithConst(Ldia, threshold);
    load<Star>(R10);
    loadWithConst(Ldia, static_cast<int64_t>(0));
    load<Add>(R1);
    load<If_icmpne>(R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold);
}

TEST_F(ExecutorTest, If_icmpgt_cycle) {
    constexpr int64_t threshold = -200;
    loadWithConst(Ldia, static_cast<int64_t>(1));
    load<Star>(R1);
    loadWithConst(Ldia, threshold);
    load<Star>(R10);
    loadWithConst(Ldia, static_cast<int64_t>(0));
    load<Sub>(R1);
    load<If_icmpgt>(R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold);
}

TEST_F(ExecutorTest, If_icmpge_cycle) {
    constexpr int64_t threshold = -200;
    loadWithConst(Ldia, static_cast<int64_t>(1));
    load<Star>(R1);
    loadWithConst(Ldia, threshold);
    load<Star>(R10);
    loadWithConst(Ldia, static_cast<int64_t>(0));
    load<Sub>(R1);
    load<If_icmpge>(R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold - 1);
}

TEST_F(ExecutorTest, If_icmplt_cycle) {
    constexpr int64_t threshold = 200;
    loadWithConst(Ldia, static_cast<int64_t>(1));
    load<Star>(R1);
    loadWithConst(Ldia, threshold);
    load<Star>(R10);
    loadWithConst(Ldia, static_cast<int64_t>(0));
    load<Add>(R1);
    load<If_icmplt>(R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold);
}

TEST_F(ExecutorTest, If_icmple_cycle) {
    constexpr int64_t threshold = 200;
    loadWithConst(Ldia, static_cast<int64_t>(1));
    load<Star>(R1);
    loadWithConst(Ldia, threshold);
    load<Star>(R10);
    loadWithConst(Ldia, static_cast<int64_t>(0));
    load<Add>(R1);
    load<If_icmple>(R10, static_cast<Immidiate>(-1 * sizeof(bytecode_t)));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold + 1);
}

TEST_F(ExecutorTest, Cmpgf_greater) {
    loadWithConst(Ldiaf, 0.1234567);
    load<Star>(R1);
    loadWithConst(Ldiaf, 123.9);
    // acc > r1
    load<Сmpgf>(R1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), 1);
}

TEST_F(ExecutorTest, Cmpgf_less) {
    loadWithConst(Ldiaf, 0.1234567);
    load<Star>(R1);
    loadWithConst(Ldiaf, -123.9);
    // acc < r1
    load<Сmpgf>(R1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), -1);
}

TEST_F(ExecutorTest, Cmpgf_equal) {
    loadWithConst(Ldiaf, 0.123456789);
    load<Star>(R1);
    loadWithConst(Ldiaf, 0.123456789);
    // acc < r1
    load<Сmpgf>(R1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), 0);
}

TEST_F(ExecutorTest, Cmpgf_nan) {
    loadWithConst(Ldiaf, NAN);
    load<Star>(R1);
    loadWithConst(Ldiaf, 0.123456789);
    // r1 is NaN
    load<Сmpgf>(R1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), -1);
}

TEST_F(ExecutorTest, Cmplf_less) {
    loadWithConst(Ldiaf, 0.15);
    load<Star>(R1);
    loadWithConst(Ldiaf, -1234.5);
    // acc < r1
    load<Cmplf>(R1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), 1);
}

TEST_F(ExecutorTest, Cmplf_nan) {
    loadWithConst(Ldiaf, 0.15);
    load<Star>(R1);
    loadWithConst(Ldiaf, NAN);
    // acc is NaN
    load<Cmplf>(R1);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), -1);
}

TEST_F(ExecutorTest, Goto_forward) {
    constexpr int ret = 345;
    load<Goto>((ret) * sizeof(bytecode_t));
    for (int i = 0; i < ret - 1; ++i) {
        load<Inv>();
    }
    ASSERT_EQ(load<Ret>(), ret);
    for (int i = 0; i < 500; ++i) {
        load<Inv>();
    }
    update();
    exec_.run();
}

TEST_F(ExecutorTest, Goto_forward_and_back) {
    // jump to
    constexpr int ret1 = 345;
    constexpr int ret2 = ret1 + 5;
    load<Goto>((ret2) * sizeof(bytecode_t));
    for (int i = 0; i < ret1 - 1; ++i) {
        load<Inv>();
    }
    ASSERT_EQ(load<Ret>(), ret1);
    load<Inv>();
    load<Inv>();
    load<Inv>();
    load<Inv>();
    ASSERT_EQ(load<Goto>(static_cast<Immidiate>(-5 * sizeof(bytecode_t))),
              ret2);
    for (int i = 0; i < 500; ++i) {
        load<Inv>();
    }
    update();
    exec_.run();
}

/*
 * r99 = val2
 * r98 = val1
 * aboba(r98, r99) {ret r49 - r48}
 * ret
 */
TEST_F(ExecutorTest, Call) {
    auto val1 = static_cast<int64_t>(314);
    auto val2 = static_cast<int64_t>(271);
    loadWithConst(Ldia, val2);
    load<Star>(99);
    loadWithConst(Ldia, val1);
    load<Star>(98);
    Immidiate func_ref =
        chaiFile_.addFunction(UINT16_MAX, "aboba_func", "(II)I",
                              std::vector<bytecode_t>{
                                  instr2Raw<Ldra>(49), // val2
                                  instr2Raw<Sub>(48),  // val1
                                  instr2Raw<Ret>(),
                              },
                              2, 50);
    load<Call>(func_ref);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), val2 - val1);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

/*
 * r3 = 23
 * acc = 100
 * i64[] acc = new i64[acc]
 * acc = acc[r3]
 */
TEST_F(ExecutorTest, GetI64FromArr) {
    constexpr auto i = static_cast<int64_t>(23);
    constexpr auto size = static_cast<int64_t>(50);
    loadWithConst(Ldia, i);
    load<Star>(R3);
    loadWithConst(Ldia, size);
    load<NewI64Array>();
    load<GetI64FromArr>(R3);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), 0);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

/*
 * r3 = 23 //i
 * r4 = 12345
 * acc = 100
 * i64[] acc = new i64[acc]
 * acc = acc[r3]
 */
TEST_F(ExecutorTest, SetI64SetArr) {
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
 * r3 = 23
 * acc = 100
 * i64[] acc = new а64[acc]
 * acc = acc[r3]
 * acc == 0.0?
 */
TEST_F(ExecutorTest, GetF64FromArr) {
    constexpr auto i = static_cast<int64_t>(23);
    constexpr auto size = static_cast<int64_t>(50);
    loadWithConst(Ldia, i);
    load<Star>(R3);
    loadWithConst(Ldia, size);
    load<NewF64Array>();
    load<GetF64FromArr>(R3);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), 0.0);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

/*
 * r3 = 23 //i
 * r4 = 12.345
 * acc = 100
 * i64[] acc = new f64[acc]
 * acc = acc[r3]
 */
TEST_F(ExecutorTest, SetF64SetArr) {
    constexpr auto i = static_cast<int64_t>(23);
    constexpr auto size = static_cast<int64_t>(50);
    constexpr auto value = static_cast<double>(12.345);
    loadWithConst(Ldia, i);
    load<Star>(R3);
    loadWithConst(Ldiaf, value);
    load<Star>(R4);
    loadWithConst(Ldia, size);
    load<NewF64Array>();
    load<SetF64InArr>(R3, R4);
    load<GetF64FromArr>(R3);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(std::bit_cast<double>(exec_.acc()), value);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(ExecutorTest, StringPrint) {
    Immidiate raw = chaiFile_.addConst(std::make_unique<ConstRawStr>("ABOBA"));
    load<Ldia>(raw);
    load<StringPrint>();
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(codeManager_.getCnstStringByReg(exec_.acc()), "ABOBA");
    EXPECT_EQ(codeManager_.getCnstStringByImm(raw), "ABOBA");
}

TEST_F(ExecutorTest, StringConcat) {
    Immidiate raw1 = chaiFile_.addConst(std::make_unique<ConstRawStr>("ABOBA"));
    Immidiate raw2 =
        chaiFile_.addConst(std::make_unique<ConstRawStr>(" Yeash"));
    load<Ldia>(raw2);
    load<StringPrint>();
    load<Star>(R2);
    load<Ldia>(raw1);
    load<StringPrint>();
    load<StringConcat>(R2);
    load<Ret>();
    update();
    EXPECT_EQ(codeManager_.getCnstStringByImm(raw1), "ABOBA");
    EXPECT_EQ(codeManager_.getCnstStringByImm(raw2), " Yeash");
    exec_.run();
    EXPECT_EQ(codeManager_.getCnstStringByReg(exec_.acc()), "ABOBA Yeash");
}

TEST_F(ExecutorTest, StringSize) {
    Immidiate raw = chaiFile_.addConst(std::make_unique<ConstRawStr>("ABOBA"));
    load<Ldia>(raw);
    load<StringLen>();
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), 5);
}

TEST_F(ExecutorTest, StringSlice) {
    Immidiate raw = chaiFile_.addConst(std::make_unique<ConstRawStr>("ABOBA"));
    constexpr auto start = static_cast<int64_t>(1);
    constexpr auto end = static_cast<int64_t>(4);
    loadWithConst(Ldia, start);
    load<Star>(R2);
    loadWithConst(Ldia, end);
    load<Star>(R3);
    load<Ldia>(raw);
    load<StringPrint>();
    load<StringSlice>(R2, R3);
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(codeManager_.getCnstStringByReg(exec_.acc()), "BOB");
    EXPECT_EQ(codeManager_.getCnstStringByImm(raw), "ABOBA");
}
