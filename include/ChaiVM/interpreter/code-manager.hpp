#pragma once
#include "ChaiVM/types.hpp"
#include "instruction.hpp"
#include <vector>
#include <filesystem>
#include <fstream>
#include <istream>
#include <iostream>

namespace chai::interpreter {

    class CodeManager {
    public:
        void load(std::istream &istream) {
            bytecode_t bytecode;
            while (istream >> bytecode) {
                raw_.push_back(bytecode);
            }
        }

        void load(const std::filesystem::path &path) {
            std::ifstream inputFile(path);
            if (inputFile.is_open()) {
                load(inputFile);
                inputFile.close();
            } else {
                throw std::invalid_argument(
                    std::string{"Invalid path "} + path.string()
                );
            }
        }

        bytecode_t getBytecode(chsize_t pc) {
            if (pc / sizeof(bytecode_t) > raw_.size() || pc % sizeof(bytecode_t) != 0) {
                return Inv;
            } else {
                return raw_[pc / sizeof(bytecode_t)];
            }
        }

        chsize_t StartPC() {
            return 0;
        }

    private:
        std::vector<bytecode_t> raw_;
    };
} // namespace chai::interpreter
