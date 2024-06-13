#include "ChaiVM/interpreter/executor.hpp"
#include "frontend/assembler/assembler.hpp"
#include <filesystem>
#include <gtest/gtest.h>

using namespace front::assembler;
using chai::interpreter::Frame;

class AssemblerTest : public ::testing::Test {
protected:
    size_t numOfRegs_ = 100;
    size_t numOfFrames_ = 5;
    chai::interpreter::CodeManager codeManager_;
    chai::memory::LinearBuffer frameBuffer_ = chai::memory::LinearBuffer(
        numOfFrames_ * (numOfRegs_ * sizeof(chai::chsize_t) + sizeof(Frame)));
    chai::memory::LinearBuffer primitivesBuffer =
        chai::memory::LinearBuffer(1024 * 256);
    chai::memory::TracedByteAllocator objectsAlocator{1024 * 256};
    chai::interpreter::Executor exec_{&codeManager_, frameBuffer_, primitivesBuffer,
                                      objectsAlocator};
    std::filesystem::path input_ = "./asm.chai";
    std::ofstream writeInput_{input_, std::ios::out};
    std::filesystem::path output_ = "./bytecode.ch";
};

TEST_F(AssemblerTest, integerMul) {
    int a = 10;
    int b = 8;
    writeInput_ << "Ldia " << a << "\n"
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
    writeInput_ << "Ldiaf " << a << "\n"
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
    writeInput_ << "Ldia \" world\"\n"
                << "Star r2\n"
                << "Ldia \"Hello\"\n"
                << "StringConcat r2\n"
                << "Ret" << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(codeManager_.getStringByStringPoolPos(exec_.acc()),
              "Hello world");
}

TEST_F(AssemblerTest, simpleFunction) {
    writeInput_ << "Ldia 271\n"
                << "Ldia 228\n"
                << "Call stupid\n"
                << "Ret\n"
                << "fn stupid 0 0 {\n"
                << "    Ldia 125\n"
                << "    Ret\n"
                << "}\n"
                << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), 125);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(AssemblerTest, squareFunctions) {
    uint64_t value = 322;
    writeInput_ << "Ldia " << value << "\n"
                << "Call square\n"
                << "Ret\n"
                << "\n"
                << "fn square 1 0 {\n"
                << "    Star r0\n"
                << "    Mul r0\n"
                << "    Ret\n"
                << "}\n"
                << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    exec_.run();
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), value * value);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}

TEST_F(AssemblerTest, noStackOverflow) {
    /**
     * todo: figure out why exactly this number of iterations is maximum for
     * our VM.
     */
    uint64_t iterations = 10000;
    writeInput_ << "Ldia " << iterations << "\n"
                << "Star r0\n"
                << "Ldia 0\n"
                << "Addi 1\n"
                << "Call aboba\n"
                << "If_icmplt r0, -2\n"
                << "Ret\n"
                << "\n"
                << "fn aboba " << numOfRegs_ << " 0 {\n"
                << "    Ret\n"
                << "}\n"
                << std::endl;
    Assembler asM{input_, output_};
    asM.assemble();
    codeManager_.load(output_);
    EXPECT_NO_THROW(exec_.run());
    EXPECT_EQ(static_cast<int64_t>(exec_.acc()), iterations);
    EXPECT_EQ(exec_.getCurrentFrame(), nullptr);
}
