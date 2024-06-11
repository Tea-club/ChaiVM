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
        instr2Raw<Ldia>(6), instr2Raw<Star>(2), instr2Raw<Ldia>(8),
        instr2Raw<Star>(3), instr2Raw<Ldra>(3), instr2Raw<Mul>(2),
        instr2Raw<Ret>()};
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

TEST_F(CodeManagerTest, loadFileWithKlass) {
    ChaiFile chai_file{std::vector<chai::bytecode_t>{defaultInstructions_}, std::vector<std::unique_ptr<Constant>>{}};
    Immidiate bar_klass = chai_file.registerKlass("Bar");
    Immidiate foo_klass = chai_file.registerKlass("Foo");
    chai_file.addField(foo_klass, "fooNum", 0U, FieldTag::I64);
    chai_file.addField(bar_klass, "myFoo", 1U, foo_klass);
    chai_file.toFile(filepath_);

    codeManager_.load(filepath_);
    EXPECT_EQ(codeManager_.getCnstStringByImm(codeManager_.getKlass(bar_klass).name_), "Bar");
    EXPECT_EQ(codeManager_.getCnstStringByImm(codeManager_.getKlass(foo_klass).name_), "Foo");
    chai::chsize_t pc = 0;
    for (const auto &ins : defaultInstructions_) {
        EXPECT_EQ(codeManager_.getBytecode(MAIN_FUNC_ID, pc), ins);
        pc += sizeof(chai::bytecode_t);
    }
    EXPECT_THROW(
        codeManager_.getBytecode(MAIN_FUNC_ID, pc + sizeof(chai::bytecode_t)),
        BeyondCodeBoundaries);
}
