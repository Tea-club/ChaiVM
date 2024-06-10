#include <gtest/gtest.h>

#include "ChaiVM/utils/file-format/chai-file.hpp"

using chai::bytecode_t;
using chai::utils::instr2Raw;
using chai::utils::fileformat::ChaiFile;
using namespace chai::interpreter;

TEST(ChaiFile, nextFunc) {
    ChaiFile chaiFile{};
    chaiFile.addWithConst(chai::interpreter::Ldiaf, static_cast<double>(0.1));
    Immidiate func_ref = chaiFile.nextFunc();
    EXPECT_EQ(chaiFile.addFunction(UINT16_MAX, "recursion", "()V",
                                   std::vector<bytecode_t>{
                                       instr2Raw<Call>(func_ref),
                                   },
                                   2, 8),
              func_ref);
}
