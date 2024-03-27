#include <gtest/gtest.h>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "frontend/assembler/op-with-info.hpp"

TEST(OpString, SringConstructor) {
    std::string inv = "Inv";
    OperationWithInfo op(inv);
    EXPECT_EQ(op, chai::interpreter::Inv);
    EXPECT_FALSE(op != chai::interpreter::Inv);
}

TEST(OpString, toString) {
    OperationWithInfo op(chai::interpreter::Ldra);
    EXPECT_EQ(op.toString(), "Ldra");
    EXPECT_EQ(op.toString(), std::string("Ldra"));
}

TEST(OpString, CastToValue) {
    OperationWithInfo op(chai::interpreter::Add);
    EXPECT_EQ(static_cast<chai::interpreter::Operation>(op),
              chai::interpreter::Add);
}

TEST(OpString, SpaceshipOperator) {
    OperationWithInfo op1(chai::interpreter::Muli);
    OperationWithInfo op2(chai::interpreter::Muli);
    EXPECT_EQ(op1, op2);
}
