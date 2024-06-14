#include <gtest/gtest.h>
#include "executor-test-fixture.hpp"

using namespace chai;
using namespace interpreter;
using namespace utils::fileformat;

TEST_F(ExecutorTest, CollectRoots) {
    auto bar_klass = chaiFile_.registerKlass("Bar");
    chaiFile_.addField(bar_klass, "bar.num1", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num2", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num3", 0U, FieldTag::I64);
    chaiFile_.addField(bar_klass, "bar.num4", 0U, FieldTag::I64);
    constexpr int64_t threshold = 1000;
    //Immidiate zero_imm = chaiFile_.addConst(std::make_unique<ConstI64>(0));
    Immidiate one_imm = chaiFile_.addConst(std::make_unique<ConstI64>(1));
    Immidiate threshold_imm = chaiFile_.addConst(std::make_unique<ConstI64>(threshold));

    load<Ldia>(one_imm);
    load<Star>(R1);
    load<Ldia>(threshold_imm);
    load<Star>(R10);
    load<AllocRef>(bar_klass);
    load<Ldra>(R2);
    load<Add>(R1);
    load<Star>(R2);
    load<If_icmplt>(R10, static_cast<Immidiate>(-4));
    load<Ret>();
    update();
    exec_.run();
    EXPECT_EQ(exec_.acc(), threshold);
}
