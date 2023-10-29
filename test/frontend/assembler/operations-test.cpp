#include <gtest/gtest.h>

#include "frontend/assembler/operations.hpp"

TEST(Operation, SringConstructor) {
    std::string inv = "Inv";
    Operation op(inv);
    EXPECT_EQ(op, Operation::Inv);
    EXPECT_FALSE(op != Operation::Inv);
}

TEST(Operation, toString) {
    Operation op(Operation::Ldra);
    EXPECT_EQ(op.toString(), "Ldra");
    EXPECT_EQ(op.toString(), std::string("Ldra"));
}

TEST(Operation, CastToValue) {
    Operation op(Operation::Add);
    EXPECT_EQ(static_cast<Operation::Value>(op), Operation::Add);
}

TEST(Operation, SpaceshipOperator) {
    Operation op1(Operation::Muli);
    Operation op2(Operation::Muli);
    EXPECT_EQ(op1, op2);
}
