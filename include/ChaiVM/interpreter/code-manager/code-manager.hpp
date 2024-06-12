#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <istream>
#include <vector>

#include "ChaiVM/interpreter/code-manager/func-struct.hpp"
#include "ChaiVM/memory/allocator.hpp"
#include "ChaiVM/memory/linear-allocator.hpp"
#include "ChaiVM/memory/linear-buffer.hpp"
#include "klass.hpp"

namespace chai::interpreter {

/**
 * Class to manage bytecode. Similar to ClassLoader.
 */
class CodeManager final {
public:
    enum ConstantTag : uint8_t {
        CNST_I64 = 1,
        CNST_F64,
        CNST_FUNC_NAME_AND_TYPE,
        CNST_RAW_STR, // Analogue of CONSTANT_Utf8 in jvm
    };

    /**
     * Parses and loads the full file.
     * @param path Path to the .chai file.
     */
    void load(const std::filesystem::path &path);

    /**
     * Loads stream of Constant pool.
     * @param istream
     */
    void loadPool(std::istream &istream);

    void loadKlass(std::istream &istream);

    void loadFunction(std::istream &istream);

    chsize_t getCnst(Immidiate id);

    /**
     * Get string by immediate, i.e. its number in constant pool.
     * @param imm Number in constant pool.
     * @return String.
     */
    const std::string &getCnstStringByImm(Immidiate imm);

    /**
     * Get string by constant that provided constant pool.
     * Usually we take constant from constant pool directly. In case of string
     * there is just 64-bit number that encodes String. Use this method to get
     * string by this number.
     * @param reg_val Number from constant pool.
     * @return String.
     */
    const std::string &getStringByStringPoolPos(chsize_t reg_val);

    Immidiate addCnstString(std::string &&str);

    bytecode_t getBytecode(size_t func, chsize_t pc);

    const Klass &getKlass(Immidiate imm) const;

    const Function &getFunc(Immidiate imm) const;

    const Function &getStartFunc() const;

private:
    /**
     * Loaded klasses.
     */
    std::vector<Klass> klasses_;

    std::vector<Function> funcs_;

    /**
     * Runtime constant pool. Constants(excepting Strings) with id [imm] can be
     * retrieved via this vector. If [imm] is String then the string keeps in
     * stringPool_[constantPool_[imm]].
     */
    std::vector<chsize_t> constantPool_;

    /**
     * We cannot contain strings in constantPool_ so we keep constant strings
     * here.
     */
    std::vector<std::string> stringPool_;
};

class BeyondCodeBoundaries : public std::runtime_error {
public:
    BeyondCodeBoundaries(char const *msg);
    BeyondCodeBoundaries(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace chai::interpreter
