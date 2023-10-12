#include "ChaiVM/interpreter/code-manager.hpp"
#include <gtest/gtest.h>

using namespace chai::interpreter;

class CodeManagerTest : public ::testing::Test {
protected:
    void fillFileWithCode(const std::filesystem::path &path, const std::vector<chai::bytecode_t> &rawInstructions) {
        std::ofstream ofs(path, std::ios::binary | std::ios::out);
        if (ofs.good() && ofs.is_open()) {
            for(const auto& ins : rawInstructions) {
                ofs.write(reinterpret_cast<const char*>(&ins), sizeof(chai::bytecode_t));
            }
            ofs.close();
        } else {
            throw std::invalid_argument(std::string{"Invalid path "} +
                                        path.string());
        }
    }
    chai::bytecode_t instr2Raw(Operation op, RegisterId r1, RegisterId r2) {
        return (operation2opcode(op)) | (r1 << 8) | (r2 << 16);
    }
    chai::bytecode_t instr2Raw(Operation op, Immidiate imm) {
        return (operation2opcode(op)) |
               (static_cast<chai::bytecode_t>(imm) << 8);
    }
    chai::bytecode_t instr2Raw(Operation op) { return (operation2opcode(op)); }

    std::vector<chai::bytecode_t> defaultInstructions_ = {
        instr2Raw(Ldia, 6),    instr2Raw(Star, 2, 0), instr2Raw(Ldia, 8),
        instr2Raw(Star, 3, 0), instr2Raw(Ldra, 3, 0), instr2Raw(Mul, 2, 0),
        instr2Raw(Ret, 0, 0)};
    CodeManager codeManager_;

private:
    uint8_t operation2opcode(Operation operation) { return (uint8_t)operation; }
};

//TEST_F(CodeManagerTest, returnsBytecodes) {
//    CodeManager manager{};
//    std::vector<Operation> seq = {Mov, Mul, Addi, Add};
//    for (const auto &op : seq) {
//        manager.load(static_cast<chai::bytecode_t>(op));
//    }
//
//    const chai::chsize_t initial_pc = manager.startPC();
//    for (int i = 0; i < seq.size(); ++i) {
//        chai::chsize_t pc = initial_pc + i * sizeof(chai::bytecode_t);
//        EXPECT_EQ(manager.getBytecode(pc), seq[i]);
//    }
//}
//
//TEST_F(CodeManagerTest, loadStringstream) {
//    char arr[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
//    std::istringstream stream{arr};
//    chai::chsize_t inst = 0;
//    stream.read(reinterpret_cast<char *>(&inst), sizeof(inst));
//    EXPECT_EQ(inst, std::bit_cast<chai::chsize_t>(0x0101010101010101));
//}

TEST_F(CodeManagerTest, loadFileDefault) {
    fillFileWithCode("./example.chai", defaultInstructions_);
    codeManager_.load("./example.chai");
    chai::chsize_t pc = codeManager_.startPC();
    for(const auto& ins : defaultInstructions_) {
        pc += sizeof(chai::bytecode_t);
        EXPECT_EQ(codeManager_.getBytecode(pc), ins);
    }
}
