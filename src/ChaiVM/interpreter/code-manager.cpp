#include <bit>
#include <cassert>

#include "ChaiVM/interpreter/code-manager.hpp"

namespace chai::interpreter {

void CodeManager::load(bytecode_t bytecode) { raw_.push_back(bytecode); }

void CodeManager::load(std::istream &istream) {
    if (!istream.good()) {
        throw std::invalid_argument(std::string{"Bad input stream"});
    }
    bytecode_t bytecode = 0;
    while (
        istream.read(reinterpret_cast<char *>(&bytecode), sizeof(bytecode_t))) {
        raw_.push_back(bytecode);
    }
}

void CodeManager::loadPool(std::istream &istream) {
    if (!istream.good()) {
        throw std::invalid_argument(std::string{"Bad input stream"});
    }
    uint16_t constants = 0;
    static_assert(sizeof constants == 2);
    static_assert(sizeof(char) == 1);
    istream.read(reinterpret_cast<char *>(&constants), sizeof constants);
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
        load(input_file);
        input_file.close();
    } else {
        throw std::invalid_argument(std::string{"Invalid path "} +
                                    path.string());
    }
}

chsize_t CodeManager::getCnst(chsize_t id) {
    assert(id < constantPool_.size());
    return constantPool_[id];
}

bytecode_t CodeManager::getBytecode(chsize_t pc) {
    if (pc / sizeof(bytecode_t) >= raw_.size() ||
        pc % sizeof(bytecode_t) != 0) {
        throw BeyondCodeBoundaries(
            "Going beyond the boundaries of the code at pc: " +
            std::to_string(pc) + " / " +
            std::to_string(raw_.size() * sizeof(bytecode_t)));
    } else {
        return raw_[pc / sizeof(bytecode_t)];
    }
}

chsize_t CodeManager::startPC() { return 0; }

BeyondCodeBoundaries::BeyondCodeBoundaries(const char *msg)
    : runtime_error(msg) {}
BeyondCodeBoundaries::BeyondCodeBoundaries(const std::string &msg)
    : runtime_error(msg) {}
const char *BeyondCodeBoundaries::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
