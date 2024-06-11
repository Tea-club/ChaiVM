#include <bit>
#include <cassert>

#include "ChaiVM/interpreter/code-manager/code-manager.hpp"
#include "ChaiVM/utils/io-bytes.hpp"

namespace chai::interpreter {

using chai::utils::readBytes;

void CodeManager::load(const std::filesystem::path &path) {
    std::ifstream input_file(path, std::ios::binary | std::ios::in);
    if (input_file.good() && input_file.is_open()) {
        loadPool(input_file);
        auto klass_count = readBytes<Immidiate>(input_file);
        for (int i = 0; i < klass_count; ++i) {
            loadKlass(input_file);
        }
        auto func_count = readBytes<Immidiate>(input_file);
        for (int i = 0; i < func_count; ++i) {
            loadFunction(input_file);
        }
        input_file.close();
    } else {
        throw std::invalid_argument(std::string{"Invalid path "} +
                                    path.string());
    }
}

void CodeManager::loadPool(std::istream &istream) {
    if (!istream.good()) {
        throw std::invalid_argument(std::string{"Bad input stream"});
    }
    Immidiate constant_count = readBytes<Immidiate>(istream);
    for (int i = 0; i < constant_count; ++i) {
        ConstantTag type = readBytes<ConstantTag>(istream);
        switch (type) {
        case CNST_I64: {
            int64_t next_long = readBytes<int64_t>(istream);
            constantPool_.push_back(next_long);
            break;
        }
        case CNST_F64: {
            double next_d = readBytes<double>(istream);
            constantPool_.push_back(std::bit_cast<chsize_t>(next_d));
            break;
        }
        case CNST_FUNC_NAME_AND_TYPE: {
            Immidiate name_index = readBytes<Immidiate>(istream);
            Immidiate descriptor_index = readBytes<Immidiate>(istream);
            constantPool_.push_back(
                (static_cast<chsize_t>(type) << 32) |
                (static_cast<chsize_t>(name_index) << 16) |
                (static_cast<chsize_t>(descriptor_index) << 0));
            break;
        }
        case CNST_RAW_STR: {
            uint16_t len = readBytes<uint16_t>(istream);
            std::unique_ptr<char[]> buf{new char[len + 1]};
            istream.read(buf.get(), len);
            buf[len] = 0;
            stringPool_.emplace_back(buf.get());
            constantPool_.push_back(stringPool_.size() - 1);
            break;
        }
        default:
            throw std::invalid_argument(std::string{"Type cannot be "} +
                                        std::to_string(type));
            break;
        }
    }
}

inline bool typeIsPrimitive(uint8_t type) { return type == 0; }

Field loadField(std::istream &istream) {
    auto name = readBytes<Immidiate>(istream);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
    auto type = readBytes<uint8_t>(istream);
    if (typeIsPrimitive(type)) {
        readBytes<uint8_t>(istream);
        char intOrFloat = readBytes<char>(istream);
    } else {
        auto klassNum = readBytes<Immidiate>(istream);
    }
#pragma GCC diagnostic pop
    return Field{name};
}

void CodeManager::loadKlass(std::istream &istream) {
    auto name = readBytes<Immidiate>(istream);
    auto fields_count = readBytes<uint8_t>(istream);
    Klass klass{name, std::vector<Field>{}};
    for (int i = 0; i < fields_count; ++i) {
        klass.fields_.push_back(loadField(istream));
    }
    klasses_.push_back(klass);
}

/*
 * @todo #1:90min We can use chai::utils::fileformat::FuncInfo here somehow.
 * Read to structure and then work with their fields.
 */
/*
 * @todo #87:90min Do something with unused variables. Don't forget to disable
 * diagnostic suppress.
 */
void CodeManager::loadFunction(std::istream &istream) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
    uint16_t access_flags = readBytes<uint16_t>(istream);
    Immidiate const_ref = readBytes<Immidiate>(istream);
    Immidiate atts_count = readBytes<Immidiate>(istream);
    // only "Code" attribute is supported
    assert(atts_count == 1);

    // read code attribute
    Immidiate att_name_index = readBytes<Immidiate>(istream);
    uint32_t att_len = readBytes<uint32_t>(istream);
    uint8_t max_regs = readBytes<uint8_t>(istream);
    uint8_t nargs = readBytes<uint8_t>(istream);
    // size of instructions in function in bytes
    uint32_t code_len = readBytes<uint32_t>(istream);
#pragma GCC diagnostic pop
    assert(code_len % sizeof(bytecode_t) == 0);
    const size_t next = funcs_.size();
    funcs_.push_back(Function{
        .numRegs = max_regs,
        .numArgs = nargs,
    });
    for (uint i = 0; i < code_len / sizeof(bytecode_t); ++i) {
        funcs_[next].code.push_back(readBytes<bytecode_t>(istream));
    }
}

chsize_t CodeManager::getCnst(Immidiate id) {
    assert(id < constantPool_.size());
    return constantPool_[id];
}

const std::string &CodeManager::getCnstStringByImm(Immidiate id) {
    return stringPool_[constantPool_[id]];
}

const std::string &CodeManager::getCnstStringByReg(chsize_t reg_val) {
    return stringPool_[reg_val];
}

Immidiate CodeManager::addCnstString(std::string &&str) {
    constantPool_.push_back(stringPool_.size());
    stringPool_.emplace_back(str);
    return constantPool_.size() - 1;
}

bytecode_t CodeManager::getBytecode(size_t func, chsize_t pc) {
    if (pc / sizeof(bytecode_t) >= funcs_[func].code.size() ||
        pc % sizeof(bytecode_t) != 0) {
        throw BeyondCodeBoundaries(
            "Going beyond the boundaries of the code at pc: " +
            std::to_string(pc) + " / " +
            std::to_string(funcs_[func].code.size() * sizeof(bytecode_t)));
    } else {
        return funcs_[func].code[pc / sizeof(bytecode_t)];
    }
}

const Klass &CodeManager::getKlass(Immidiate imm) const {
    assert(klasses_.size() > imm);
    return klasses_[imm];
}

const Function &CodeManager::getFunc(Immidiate imm) const {
    return funcs_[imm];
}

const Function &CodeManager::getStartFunc() const {
    assert(!funcs_.empty());
    return funcs_[0];
}

BeyondCodeBoundaries::BeyondCodeBoundaries(const char *msg)
    : runtime_error(msg) {}
BeyondCodeBoundaries::BeyondCodeBoundaries(const std::string &msg)
    : runtime_error(msg) {}
const char *BeyondCodeBoundaries::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
