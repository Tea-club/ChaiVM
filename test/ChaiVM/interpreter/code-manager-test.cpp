#include <gtest/gtest.h>

#include "ChaiVM/interpreter/code-manager/code-manager.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"

using namespace chai::interpreter;
using namespace chai::utils;
using namespace chai::utils::fileformat;

constexpr size_t MAIN_FUNC_ID = 0;

class CodeManagerTest : public ::testing::Test {
protected:
    void fillFileWithCode(const std::filesystem::path &path,
                          std::vector<std::unique_ptr<Constant>> &&raw_pool,
                          std::vector<chai::bytecode_t> &&instructions) {
        ChaiFile chai_file{std::move(instructions), std::move(raw_pool)};
        chai_file.toFile(path);
    }

    void SetUp() override { std::remove(filepath_.c_str()); }

    void TearDown() override { std::remove(filepath_.c_str()); }

    std::vector<chai::bytecode_t> defaultInstructions_ = {
        instr2Raw(Ldia, 6),    instr2Raw(Star, 2, 0), instr2Raw(Ldia, 8),
        instr2Raw(Star, 3, 0), instr2Raw(Ldra, 3, 0), instr2Raw(Mul, 2, 0),
        instr2Raw(Ret, 0, 0)};
    CodeManager codeManager_;
    std::filesystem::path filepath_ = "./example.chai";
};

TEST_F(CodeManagerTest, loadFBadFile) {
    EXPECT_THROW(codeManager_.load(filepath_), std::invalid_argument);
}

TEST_F(CodeManagerTest, loadFileDefault) {
    fillFileWithCode(filepath_, std::vector<std::unique_ptr<Constant>>{},
                     std::vector<chai::bytecode_t>{defaultInstructions_});
    codeManager_.load(filepath_);
    chai::chsize_t pc = 0;
    for (const auto &ins : defaultInstructions_) {
        EXPECT_EQ(codeManager_.getBytecode(MAIN_FUNC_ID, pc), ins);
        pc += sizeof(chai::bytecode_t);
    }
    EXPECT_THROW(
        codeManager_.getBytecode(MAIN_FUNC_ID, pc + sizeof(chai::bytecode_t)),
        BeyondCodeBoundaries);
}
