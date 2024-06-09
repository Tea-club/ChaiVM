#include "ChaiVM/interpreter/executor.hpp"
#include "frontend/assembler/assembler.hpp"
#include <filesystem>
#include <gtest/gtest.h>

using namespace front::assembler;

class AssemblerTest : public ::testing::Test {
protected:
    chai::interpreter::CodeManager codeManager_;
    chai::memory::LinearBuffer buffer_ = chai::memory::LinearBuffer(1024 * 256);
    chai::interpreter::Executor exec_{&codeManager_, buffer_};
    std::filesystem::path input_ = "./asm.chai";
    std::ofstream write_input_{input_, std::ios::out};
    std::filesystem::path output_ = "./bytecode.ch";
};

TEST_F(AssemblerTest, run) {
    write_input_ << "Ldia 6\n"
                 << "Star r2\n"
                 << "Ldia 8\n"
                 << "Star r3\n"
                 << "Ldra r3\n"
                 << "Mul r2\n"
                 << "Ret" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(exec_.acc(), 48);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(AssemblerTest, runWithStrings) {
    write_input_ << "Ldia \" world\"\n"
                 << "Star r2\n"
                 << "Ldia \"Hello\"\n"
                 << "StringConcat r2\n"
                 << "Ret" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(codeManager_.getCnstString(exec_.acc()), "Hello world");
}

TEST_F(AssemblerTest, runWithFunctions) {
    write_input_ << "Ldia 271\n"
                 << "Ldia 228\n"
                 << "Call 1\n"
                 << "Ret\n"
                 << "fn aboba_func 50 2 {\n"
                 << "Ldia 125\n"
                 << "Ret\n"
                 << "}\n" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), 125);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
