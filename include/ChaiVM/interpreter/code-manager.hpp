#pragma once
#include "ChaiVM/types.hpp"
#include "instruction.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

namespace chai::interpreter {

class CodeManager {
public:
    void load(bytecode_t bytecode);

    void load(std::istream &istream);

    void load(const std::filesystem::path &path);

    bytecode_t getBytecode(chsize_t pc);

    chsize_t startPC();

private:
    std::vector<bytecode_t> raw_;
};

} // namespace chai::interpreter
