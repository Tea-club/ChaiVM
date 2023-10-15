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

void CodeManager::load(const std::filesystem::path &path) {
    std::ifstream inputFile(path, std::ios::binary | std::ios::in);
    if (inputFile.good() && inputFile.is_open()) {
        load(inputFile);
        inputFile.close();
    } else {
        throw std::invalid_argument(std::string{"Invalid path "} +
                                    path.string());
    }
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
