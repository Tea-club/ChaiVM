#include "ChaiVM/interpreter/code-manager.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"
#include <gtest/gtest.h>

using namespace chai::interpreter;
using namespace chai::utils;

class CodeManagerTest : public ::testing::Test {
protected:
    void
    fillFileWithCode(const std::filesystem::path &path,
                     const std::vector<chai::bytecode_t> &rawInstructions) {
        std::ofstream ofs(path, std::ios::binary | std::ios::out);
        if (ofs.good() && ofs.is_open()) {
            for (const auto &ins : rawInstructions) {
                ofs.write(reinterpret_cast<const char *>(&ins),
                          sizeof(chai::bytecode_t));
            }
            ofs.close();
        } else {
            throw std::invalid_argument(std::string{"Invalid path "} +
                                        path.string());
        }
    }

    void TearDown() override { std::remove(filepath_.c_str()); }

    std::vector<chai::bytecode_t> defaultInstructions_ = {
        instr2Raw(Ldia, 6),    instr2Raw(Star, 2, 0), instr2Raw(Ldia, 8),
        instr2Raw(Star, 3, 0), instr2Raw(Ldra, 3, 0), instr2Raw(Mul, 2, 0),
        instr2Raw(Ret, 0, 0)};
    CodeManager codeManager_;
    std::filesystem::path filepath_ = "./example.chai";

private:
    uint8_t operation2opcode(Operation operation) { return (uint8_t)operation; }
};

TEST(CodeManager, returnsBytecodes) {
    CodeManager manager{};
    std::vector<Operation> seq = {Mov, Mul, Addi, Add};
    for (const auto &op : seq) {
        manager.load(static_cast<chai::bytecode_t>(op));
    }

    const chai::chsize_t initial_pc = manager.startPC();
    for (int i = 0; i < seq.size(); ++i) {
        chai::chsize_t pc = initial_pc + i * sizeof(chai::bytecode_t);
        EXPECT_EQ(manager.getBytecode(pc), seq[i]);
    }
}

TEST_F(CodeManagerTest, loadFBadFile) {
    EXPECT_THROW(codeManager_.load(filepath_), std::invalid_argument);
}

TEST_F(CodeManagerTest, loadFileDefault) {
    fillFileWithCode(filepath_, defaultInstructions_);
    codeManager_.load(filepath_);
    chai::chsize_t pc = codeManager_.startPC();
    for (const auto &ins : defaultInstructions_) {
        EXPECT_EQ(codeManager_.getBytecode(pc), ins);
        pc += sizeof(chai::bytecode_t);
    }
    EXPECT_THROW(codeManager_.getBytecode(pc + sizeof(chai::bytecode_t)),
                 BeyondCodeBoundaries);
}
