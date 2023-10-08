#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"

using namespace chai::interpreter;

class ExecutorTest : public ::testing::Test {
protected:
    Executor exec;
};

TEST_F(ExecutorTest, inv) {
    Instruction invIns = {Inv, 0, 0, 0};
    EXPECT_THROW(exec.execute(invIns), InvalidInstruction);
}

TEST_F(ExecutorTest, ret) {
    Instruction retIns = {Ret, 0, 0, 0};
    exec.execute(retIns);
    EXPECT_EQ(exec.getState().pc(), sizeof(chai::bytecode_t));
}

/**
 * @todo #8:5m>/DEV add other tests after implemented decoder
 */
