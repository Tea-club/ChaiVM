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

    void loadFunction(std::istream &istream) {
        uint16_t access_flags = 0;
        istream.read(reinterpret_cast<char *>(&access_flags),
                     sizeof access_flags);
        Immidiate const_ref = 0;
        istream.read(reinterpret_cast<char *>(&const_ref), sizeof const_ref);
        Immidiate atts_count = 0;
        istream.read(reinterpret_cast<char *>(&atts_count), sizeof atts_count);
        // only "Code" attribute is supported
        assert(atts_count == 1);

        // read code attribute
        Immidiate att_name_index = 0;
        istream.read(reinterpret_cast<char *>(&att_name_index),
                     sizeof att_name_index);
        uint32_t att_len = 0;
        istream.read(reinterpret_cast<char *>(&att_len), sizeof att_len);
        uint8_t max_regs = 0;
        istream.read(reinterpret_cast<char *>(&max_regs), sizeof max_regs);
        uint8_t nargs = 0;
        istream.read(reinterpret_cast<char *>(&nargs), sizeof nargs);
        // size of instructions in function in bytes
        uint32_t code_len = 0;
        istream.read(reinterpret_cast<char *>(&code_len), sizeof code_len);
        std::cout << "acc_flags = " << access_flags << std::endl;
        std::cout << "const_ref = " << const_ref << std::endl;
        std::cout << "atts_count = " << atts_count << std::endl;
        std::cout << "atts_name_index = " << att_name_index << std::endl;
        std::cout << "atts_len = " << att_len << std::endl;
        std::cout << "max_regs = " << (int)max_regs << std::endl;
        std::cout << "nargs = " << (int)nargs << std::endl;
        std::cout << "codelen = " << code_len << std::endl;

        assert(code_len % sizeof(bytecode_t) == 0);
        bytecode_t bytecode = 0;
        const size_t next = funcs_.size();
        std::cout << "next = " << next << std::endl;
        funcs_.push_back(Function{
            .num_regs = max_regs,
            .num_args = nargs,
        });
        std::cout << "before cycle, size = " << funcs_[next].code.size()
                  << std::endl;
        for (uint i = 0; i < code_len / sizeof(bytecode_t); ++i) {
            istream.read(reinterpret_cast<char *>(&bytecode),
                         sizeof(bytecode_t));
            funcs_[next].code.push_back(bytecode);
            std::cout << "pushed back " << bytecode
                      << ", size = " << funcs_[next].code.size() << std::endl;
        }
        dispatch_[const_ref] = next;
    }

    chsize_t getCnst(Immidiate id);

    bytecode_t getBytecode(size_t func, chsize_t pc);

    const Function &getFunc(Immidiate imm) const {
        std::cout << "dispatch_[imm] = " << dispatch_[imm] << std::endl;
        return funcs_[dispatch_[imm]];
    }

    const Function &startFunc() const {
        assert(!funcs_.empty());
        //        std::cout << "in start func, code: " << funcs_[0].code.size()
        //        << std::endl; for (const bytecode_t &instr: funcs_[0].code ) {
        //            printf("%lu, ", instr);
        //        }
        //        std::cout << std::endl;
        return funcs_[0];
    }

private:
    std::vector<Function> funcs_;
    std::vector<chsize_t> constantPool_;
    std::vector<std::string> stringPool_{};

    /**
     * Id in appropriate collection by immidiate.
     *  For example, func by imm is found as funcs_[dispatch_[imm]].
     */
    std::vector<Immidiate> dispatch_;
};

class BeyondCodeBoundaries : public std::runtime_error {
public:
    BeyondCodeBoundaries(char const *msg);
    BeyondCodeBoundaries(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace chai::interpreter
