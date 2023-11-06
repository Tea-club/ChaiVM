#include <bit>
#include <cassert>

#include "ChaiVM/interpreter/code-manager/code-manager.hpp"

namespace chai::interpreter {

void CodeManager::loadPool(std::istream &istream) {
    if (!istream.good()) {
        throw std::invalid_argument(std::string{"Bad input stream"});
    }
    Immidiate constants = 0;
    istream.read(reinterpret_cast<char *>(&constants), sizeof constants);
    dispatch_ = std::vector<Immidiate>(constants, -1);
    std::cout << "constants = " << constants << std::endl;
    for (int i = 0; i < constants; ++i) {
        char type;
        istream.read(&type, sizeof type);
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
            constantPool_.push_back((static_cast<chsize_t>(type) << 32) |
                                    (static_cast<chsize_t>(name_index) << 16) |
                                    (static_cast<chsize_t>(name_index) << 0));
            break;
        case CNST_RAW_STR:
            uint16_t len;
            istream.read(reinterpret_cast<char *>(&len), sizeof len);
            char *buf;
            buf = new char[len + 1];
            istream.read(buf, len);
            buf[len] = 0;
            stringPool_.push_back(std::string{buf});
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

void CodeManager::load(const std::filesystem::path &path) {
    std::ifstream input_file(path, std::ios::binary | std::ios::in);
    if (input_file.good() && input_file.is_open()) {
        loadPool(input_file);
        Immidiate func_count = 0;
        input_file.read(reinterpret_cast<char *>(&func_count),
                        sizeof func_count);
        std::cout << "func_count = " << func_count << std::endl;
        funcs_ = std::vector<Function>{};
        for (int i = 0; i < func_count; ++i) {
            loadFunction(input_file);
        }
        input_file.close();
    } else {
        throw std::invalid_argument(std::string{"Invalid path "} +
                                    path.string());
    }
}

chsize_t CodeManager::getCnst(Immidiate id) {
    std::cout << "getCnst, constant pool size = " << constantPool_.size()
              << std::endl;
    std::cout << "getCnst, imm id = " << id << std::endl;
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

BeyondCodeBoundaries::BeyondCodeBoundaries(const char *msg)
    : runtime_error(msg) {}
BeyondCodeBoundaries::BeyondCodeBoundaries(const std::string &msg)
    : runtime_error(msg) {}
const char *BeyondCodeBoundaries::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
