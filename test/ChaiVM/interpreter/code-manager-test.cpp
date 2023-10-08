#include <gtest/gtest.h>
#include "ChaiVM/interpreter/code-manager.hpp"

using namespace chai::interpreter;

//  TEST(CodeManager, returnsBytecodes) {
//      CodeManager manager{};
//      constexpr Operation seq[] = {Mov, Mul, Addi, Add};
//      constexpr char arr[] = {Mov,0, 0, 0, Mul, 0, 0, 0, Addi, 0, 0, 0, Add, 0, 0, 0};
//      const std::string str{arr};
//      std::istringstream istream(str);
//      manager.load(istream);
//      EXPECT_EQ(manager.getBytecode(0), Mov);
//  }
