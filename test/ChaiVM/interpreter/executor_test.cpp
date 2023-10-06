#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"

using namespace chai::interpreter;

TEST(ExecutorTest, advancePc) {
    Executor exec;
    exec.advancePc();
    EXPECT_EQ(exec.getState().getPc(), 4);
}

TEST(ExecutorTest, ret) {
    Executor exec;
    /**
     * @todo #8:5m>/DEV fix this instruction generation after implemented
     * decoder
     */
    Instruction retIns = {static_cast<Operation>(1), 0, 0, 0};
    exec.execute(retIns);
    EXPECT_EQ(exec.getState().getPc(), 4);
}

/**
 * @todo #8:5m>/DEV add other tests after implemented decoder
 */
