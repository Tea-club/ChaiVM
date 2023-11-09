#pragma once

#include <cmath>
#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/file-format/constant.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

class ExecutorTest : public ::testing::Test {
protected:
    static constexpr chai::interpreter::RegisterId R0 = 1;
    static constexpr chai::interpreter::RegisterId R1 = 1;
    static constexpr chai::interpreter::RegisterId R2 = 2;
    static constexpr chai::interpreter::RegisterId R3 = 3;
    static constexpr chai::interpreter::RegisterId R4 = 4;
    static constexpr chai::interpreter::RegisterId R5 = 5;
    static constexpr chai::interpreter::RegisterId R6 = 6;
    static constexpr chai::interpreter::RegisterId R7 = 7;
    static constexpr chai::interpreter::RegisterId R8 = 8;
    static constexpr chai::interpreter::RegisterId R9 = 9;
    static constexpr chai::interpreter::RegisterId R10 = 10;
    static constexpr chai::interpreter::RegisterId R11 = 11;
    std::filesystem::path PATH;

    /*
     * @todo #42:60min Rename all load methods to more appropriate names.
     */
    void loadRR(chai::interpreter::Operation op,
                chai::interpreter::RegisterId reg1,
                chai::interpreter::RegisterId reg2 = 0);
    void loadRI(chai::interpreter::Operation op,
                chai::interpreter::RegisterId reg1,
                chai::interpreter::Immidiate imm);

    int loadI(chai::interpreter::Operation op,
              chai::interpreter::Immidiate imm);

    void loadWithConst(chai::interpreter::Operation op, int64_t data);

    void loadWithConst(chai::interpreter::Operation op, double data);

    int load(chai::interpreter::Operation op);

    void update();

    void SetUp() override;

    void TearDown() override;

    static std::atomic<int> counter;
    ChaiFile chaiFile_;
    chai::interpreter::CodeManager codeManager_;
    chai::memory::LinearBuffer buffer_ = chai::memory::LinearBuffer(1024 * 256);
    chai::interpreter::Executor exec_{&codeManager_, buffer_};
};
