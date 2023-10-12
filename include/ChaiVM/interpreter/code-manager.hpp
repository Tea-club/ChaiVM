#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

#include "ChaiVM/types.hpp"
#include "instruction.hpp"

namespace chai::interpreter {

/**
 * Class to manage bytecode. Similar to ClassLoader.
 */
class CodeManager {
public:
    void load(bytecode_t bytecode);

    void load(std::istream &istream);

    /**
     * Parses file by path and loads its code.
     * @param path Path of file to load.
     * @todo #8:90min Implement the method below.
     */
    void load(const std::filesystem::path &path);

    bytecode_t getBytecode(chsize_t pc);

    chsize_t startPC();

private:
    std::vector<bytecode_t> raw_;
};

class BeyondCodeBoundaries : public std::runtime_error {
public:
    BeyondCodeBoundaries(char const *msg);
    BeyondCodeBoundaries(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace chai::interpreter
