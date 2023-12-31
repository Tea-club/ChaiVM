#include <bit>
#include <cassert>

#include "ChaiVM/interpreter/code-manager/code-manager.hpp"

namespace chai::interpreter {

void CodeManager::load(const std::filesystem::path &path) {
    std::ifstream input_file(path, std::ios::binary | std::ios::in);
    if (input_file.good() && input_file.is_open()) {
        loadPool(input_file);
        Immidiate func_count;
        input_file.read(reinterpret_cast<char *>(&func_count),
                        sizeof func_count);
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
    Immidiate constant_count;
    istream.read(reinterpret_cast<char *>(&constant_count),
                 sizeof constant_count);
    dispatch_ = std::vector<Immidiate>(constant_count, -1);
    for (int i = 0; i < constant_count; ++i) {
        ConstantTag type;
        istream.read(reinterpret_cast<char *>(&type), sizeof type);
        switch (type) {
        case CNST_I64:
            int64_t next_long;
            istream.read(reinterpret_cast<char *>(&next_long),
                         sizeof next_long);
            constantPool_.push_back(next_long);
            break;
        case CNST_F64:
            double next_d;
            istream.read(reinterpret_cast<char *>(&next_d), sizeof next_d);
            constantPool_.push_back(std::bit_cast<chsize_t>(next_d));
            break;
        case CNST_FUNC_NAME_AND_TYPE:
            Immidiate name_index;
            istream.read(reinterpret_cast<char *>(&name_index),
                         sizeof name_index);
            Immidiate descriptor_index;
            istream.read(reinterpret_cast<char *>(&descriptor_index),
                         sizeof descriptor_index);
            constantPool_.push_back(
                (static_cast<chsize_t>(type) << 32) |
                (static_cast<chsize_t>(name_index) << 16) |
                (static_cast<chsize_t>(descriptor_index) << 0));
            break;
        case CNST_RAW_STR:
            uint16_t len;
            istream.read(reinterpret_cast<char *>(&len), sizeof len);
            char *buf;
            buf = new char[len + 1];
            istream.read(buf, len);
            buf[len] = 0;
            stringPool_.emplace_back(buf);
            constantPool_.push_back(stringPool_.size() - 1);
            delete[] buf;
            break;
        default:
            throw std::invalid_argument(std::string{"Type cannot be "} +
                                        std::to_string(type));
            break;
        }
    }
}

/*
 * @todo #1:90min We can use chai::utils::fileformat::FuncInfo here somehow.
 * Read to structure and then work with their fields.
 */
void CodeManager::loadFunction(std::istream &istream) {
    uint16_t access_flags;
    istream.read(reinterpret_cast<char *>(&access_flags), sizeof access_flags);
    Immidiate const_ref;
    istream.read(reinterpret_cast<char *>(&const_ref), sizeof const_ref);
    Immidiate atts_count;
    istream.read(reinterpret_cast<char *>(&atts_count), sizeof atts_count);
    // only "Code" attribute is supported
    assert(atts_count == 1);

    // read code attribute
    Immidiate att_name_index;
    istream.read(reinterpret_cast<char *>(&att_name_index),
                 sizeof att_name_index);
    uint32_t att_len;
    istream.read(reinterpret_cast<char *>(&att_len), sizeof att_len);
    uint8_t max_regs;
    istream.read(reinterpret_cast<char *>(&max_regs), sizeof max_regs);
    uint8_t nargs;
    istream.read(reinterpret_cast<char *>(&nargs), sizeof nargs);
    // size of instructions in function in bytes
    uint32_t code_len;
    istream.read(reinterpret_cast<char *>(&code_len), sizeof code_len);

    assert(code_len % sizeof(bytecode_t) == 0);
    bytecode_t bytecode;
    const size_t next = funcs_.size();
    funcs_.push_back(Function{
        .numRegs = max_regs,
        .numArgs = nargs,
    });
    for (uint i = 0; i < code_len / sizeof(bytecode_t); ++i) {
        istream.read(reinterpret_cast<char *>(&bytecode), sizeof(bytecode_t));
        funcs_[next].code.push_back(bytecode);
    }
    dispatch_[const_ref] = next;
}

chsize_t CodeManager::getCnst(Immidiate id) {
    assert(id < constantPool_.size());
    return constantPool_[id];
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

const Function &CodeManager::getFunc(Immidiate imm) const {
    return funcs_[dispatch_[imm]];
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
