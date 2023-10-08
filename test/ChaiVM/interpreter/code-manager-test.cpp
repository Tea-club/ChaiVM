#include "ChaiVM/interpreter/code-manager.hpp"
#include <gtest/gtest.h>

using namespace chai::interpreter;

TEST(CodeManager, returnsBytecodes) {
    CodeManager manager{};
    std::vector<Operation> seq = {Mov, Mul, Addi, Add};
    for (const auto &op : seq) {
        manager.load(static_cast<chai::bytecode_t>(op));
    }

    const chai::chsize_t initial_pc = manager.startPC();
    for (int i = 0; i < seq.size(); ++i) {
        chai::chsize_t pc = initial_pc + i * 4;
        EXPECT_EQ(manager.getBytecode(pc), seq[i]);
    }
}
