#pragma once

#include "constant.hpp"

class ChaiFile {
public:
    ChaiFile(std::vector<chai::bytecode_t> &&instrs,
             std::vector<std::unique_ptr<Constant>> &&pool)
        : rawInstrs_(instrs), pool_(std::move(pool)) {}

    ChaiFile()
        : ChaiFile(std::vector<chai::bytecode_t>{},
                   std::vector<std::unique_ptr<Constant>>{}) {}

    chai::interpreter::Immidiate addInstr(chai::bytecode_t raw) {
        rawInstrs_.push_back(raw);
        return rawInstrs_.size() - 1;
    }

    chai::interpreter::Immidiate addConst(std::unique_ptr<Constant> &&constant) {
        pool_.push_back(std::move(constant));
        return pool_.size() - 1;
    }

    void toFIle(const std::filesystem::path &path) {
        std::ofstream ofs(path, std::ios::binary | std::ios::out);
        if (ofs.good() && ofs.is_open()) {
            uint16_t constants = pool_.size();
            ofs.write(reinterpret_cast<const char *>(&constants),
                      sizeof(constants));
            for (const std::unique_ptr<Constant> &cnst : pool_) {
                cnst->putType(ofs);
                cnst->write(ofs);
            }
            for (const auto &ins : rawInstrs_) {
                ofs.write(reinterpret_cast<const char *>(&ins),
                          sizeof(chai::bytecode_t));
            }
            ofs.close();
        } else {
            throw std::invalid_argument(std::string{"Invalid path "} +
                                        path.string());
        }
    }

private:
    std::vector<chai::bytecode_t> rawInstrs_;
    std::vector<std::unique_ptr<Constant>> pool_;
};
