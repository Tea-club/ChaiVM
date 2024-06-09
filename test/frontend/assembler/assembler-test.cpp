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

TEST_F(AssemblerTest, integerMul) {
    int a = 10;
    int b = 8;
    write_input_ << "Ldia " << a << "\n"
                 << "Star r2\n"
                 << "Ldia " << b << "\n"
                 << "Star r3\n"
                 << "Ldra r3\n"
                 << "Mul r2\n"
                 << "Ret" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(exec_.acc(), a * b);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(AssemblerTest, floatMul) {
    double a = 6.54;
    double b = 12.13;
    write_input_ << "Ldiaf " << a << "\n"
                 << "Star r2\n"
                 << "Ldiaf " << b << "\n"
                 << "Star r3\n"
                 << "Ldra r3\n"
                 << "Mulf r2\n"
                 << "Ret" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_FLOAT_EQ(std::bit_cast<double>(exec_.acc()), a * b);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(AssemblerTest, strings) {
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

TEST_F(AssemblerTest, simpleFunction) {
    write_input_ << "Ldia 271\n"
                 << "Ldia 228\n"
                 << "Call aboba_func\n"
                 << "Ret\n"
                 << "fn aboba_func 0 0 {\n"
                 << "    Ldia 125\n"
                 << "    Ret\n"
                 << "}\n" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), 125);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(AssemblerTest, squareFunctions) {
    uint64_t value = 322;
    write_input_ << "Ldia " << value << "\n"
                 << "Call aboba_func\n"
                 << "Ret\n"
                 << "\n"
                 << "fn aboba_func 1 0 {\n"
                 << "    Star r0\n"
                 << "    Mul r0\n"
                 << "    Ret\n"
                 << "}\n" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), value * value);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
