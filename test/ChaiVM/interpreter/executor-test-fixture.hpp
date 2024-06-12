#pragma once

#include <cmath>
#include <gtest/gtest.h>

#include "ChaiVM/interpreter/executor.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/file-format/constant.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

class ExecutorTest : public ::testing::Test {
protected:
    static constexpr chai::interpreter::RegisterId R0 = 0;
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
    std::filesystem::path path_;

    /**
     * These methods loads the operation in template parameter with the
     * corresponding parameters.
     * @param op Operation.
     * @return Number of added instruction among all instructions.
     */
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::R,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::RegisterId reg1) {
        return loadRR(op, reg1, 0);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::RR,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::RegisterId reg1,
         chai::interpreter::RegisterId reg2) {
        return loadRR(op, reg1, reg2);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::RI,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::RegisterId reg1, chai::interpreter::Immidiate imm) {
        return loadRI(op, reg1, imm);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::I,
                            chai::interpreter::Immidiate>::type
    load(chai::interpreter::Immidiate imm) {
        return loadI(op, imm);
    }
    template <chai::interpreter::Operation op>
    typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] ==
                                chai::interpreter::N,
                            chai::interpreter::Immidiate>::type
    load() {
        return loadN(op);
    }

    // @todo #54:60min do something to add this functionality to load Immediate
    void loadWithConst(chai::interpreter::Operation op, int64_t data);

    void loadWithConst(chai::interpreter::Operation op, double data);

    void update();

    void SetUp() override;

    void TearDown() override;

private:
    chai::interpreter::Immidiate loadRR(chai::interpreter::Operation op,
                                        chai::interpreter::RegisterId reg1,
                                        chai::interpreter::RegisterId reg2 = 0);
    chai::interpreter::Immidiate loadRI(chai::interpreter::Operation op,
                                        chai::interpreter::RegisterId reg1,
                                        chai::interpreter::Immidiate imm);
    chai::interpreter::Immidiate loadI(chai::interpreter::Operation op,
                                       chai::interpreter::Immidiate imm);
    chai::interpreter::Immidiate loadN(chai::interpreter::Operation op);

protected:
    chai::utils::fileformat::ChaiFile chaiFile_;
    chai::interpreter::CodeManager codeManager_;
    chai::memory::LinearBuffer buffer_ = chai::memory::LinearBuffer(1024 * 256);
    // todo pass other buffer
    chai::interpreter::Executor exec_{&codeManager_, buffer_, buffer_};
};
