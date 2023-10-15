#pragma once

#include "ChaiVM/interpreter/executor.hpp"

using namespace chai::interpreter;

class CodeManWrapper {
public:
    /*
     * @todo #32:60min Make the methods "instr2Raw" implemented only once. For
     * example in utils.
     */
    static chai::bytecode_t instr2Raw(Operation op, RegisterId r1,
                                      RegisterId r2) {
        return (operation2opcode(op)) | (r1 << 8) | (r2 << 16);
    }
    static chai::bytecode_t instr2Raw(Operation op, Immidiate imm) {
        return (operation2opcode(op)) |
               (static_cast<chai::bytecode_t>(imm) << 8);
    }
    static chai::bytecode_t instr2Raw(Operation op) {
        return (operation2opcode(op));
    }

public:
    void load(Operation op, RegisterId r1, RegisterId r2) {
        manager_.load(instr2Raw(op, r1, r2));
    }

    void load(Operation op, Immidiate imm) {
        manager_.load(instr2Raw(op, imm));
    }

    void load(Operation op) { manager_.load(instr2Raw(op)); }

    static uint8_t operation2opcode(Operation operation) {
        return (uint8_t)operation;
    }

public:
    CodeManager manager_;
};
