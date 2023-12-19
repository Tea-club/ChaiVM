#include <gtest/gtest.h>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "frontend/assembler/op-string.hpp"

TEST(OpString, SringConstructor) {
    std::string inv = "Inv";
    OpString op(inv);
    EXPECT_EQ(op, chai::interpreter::Inv);
    EXPECT_FALSE(op != chai::interpreter::Inv);
}

TEST(OpString, toString) {
    OpString op(chai::interpreter::Ldra);
    EXPECT_EQ(op.toString(), "Ldra");
    EXPECT_EQ(op.toString(), std::string("Ldra"));
}

TEST(OpString, CastToValue) {
    OpString op(chai::interpreter::Add);
    EXPECT_EQ(static_cast<chai::interpreter::Operation>(op),
              chai::interpreter::Add);
}

TEST(OpString, SpaceshipOperator) {
    OpString op1(chai::interpreter::Muli);
    OpString op2(chai::interpreter::Muli);
    EXPECT_EQ(op1, op2);
}
