#include "ChaiVM/interpreter/code-manager.hpp"
#include "ChaiVM/interpreter/decoder.hpp"
using namespace chai::interpreter;

void CodeManager::load(chai::bytecode_t bytecode) { raw_.push_back(bytecode); }

void CodeManager::load(std::istream &istream) {
    bytecode_t bytecode;
    while (istream >> bytecode) {
        raw_.push_back(bytecode);
    }
}

void CodeManager::load(const std::filesystem::path &path) {
    std::ifstream inputFile(path);
    if (inputFile.is_open()) {
        load(inputFile);
        inputFile.close();
    } else {
        throw std::invalid_argument(std::string{"Invalid path "} +
                                    path.string());
    }
}

chai::bytecode_t CodeManager::getBytecode(chai::chsize_t pc) {
    if (pc / sizeof(bytecode_t) >= raw_.size() ||
        pc % sizeof(bytecode_t) != 0) {
        throw BeyondCodeBoundaries(
            "Going beyond the boundaries of the code at pc: " +
            std::to_string(pc));
    } else {
        return raw_[pc / sizeof(bytecode_t)];
    }
}

chai::chsize_t CodeManager::startPC() { return 0; }

BeyondCodeBoundaries::BeyondCodeBoundaries(const char *msg)
    : runtime_error(msg) {}
BeyondCodeBoundaries::BeyondCodeBoundaries(const std::string &msg)
    : runtime_error(msg) {}
const char *BeyondCodeBoundaries::what() const noexcept {
    return runtime_error::what();
}