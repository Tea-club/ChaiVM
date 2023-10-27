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

    /**
     * Loads one instruction.
     * @param bytecode Raw instruction.
     */
    void load(bytecode_t bytecode);

    /**
     * Loads stream of instructions only.
     * @param istream
     */
    void load(std::istream &istream);

    /**
     * Parses and loads the full file.
     * @param path Path to the .chai file.
     */
    void load(const std::filesystem::path &path);


    void loadPool(std::istream &istream);

    bytecode_t getBytecode(chsize_t pc);

    chsize_t startPC();

private:
    std::vector<bytecode_t> raw_;
    std::vector<chsize_t> constant_pool_;
};

class BeyondCodeBoundaries : public std::runtime_error {
public:
    BeyondCodeBoundaries(char const *msg);
    BeyondCodeBoundaries(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace chai::interpreter
