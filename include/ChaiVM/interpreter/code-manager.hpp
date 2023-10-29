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
class CodeManager final {
public:

    static constexpr int8_t CNST_I64 = 'l';
    static constexpr int8_t CNST_F64 = 'd';

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

    chsize_t getCnst(chsize_t id);

    bytecode_t getBytecode(chsize_t pc);

    chsize_t startPC();

private:
    std::vector<bytecode_t> raw_;
    std::vector<chsize_t> constantPool_;
};

class BeyondCodeBoundaries : public std::runtime_error {
public:
    BeyondCodeBoundaries(char const *msg);
    BeyondCodeBoundaries(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace chai::interpreter
