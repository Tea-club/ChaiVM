#pragma once

#include <cstdint>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "ChaiVM/interpreter/instruction.hpp"
#include "ChaiVM/types.hpp"

namespace chai::utils {

chai::bytecode_t instr2RawRR(interpreter::Operation op,
                             interpreter::RegisterId r1,
                             interpreter::RegisterId r2);

chai::bytecode_t instr2RawI(interpreter::Operation op,
                            interpreter::Immidiate imm);

chai::bytecode_t instr2RawN(interpreter::Operation op);

chai::bytecode_t instr2RawRI(interpreter::Operation op,
                             interpreter::RegisterId r1,
                             interpreter::Immidiate imm);

/**
 * Instr2Raw for RR formats.
 */
template <chai::interpreter::Operation op>
typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] == chai::interpreter::RR,
chai::bytecode_t>::type instr2Raw(interpreter::RegisterId r1, interpreter::RegisterId r2) {
    return instr2RawRR(op, r1, r2);
}

/**
 * Instr2Raw for R formats.
 */
template <chai::interpreter::Operation op>
typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] == chai::interpreter::R,
    chai::bytecode_t>::type instr2Raw(interpreter::RegisterId r1) {
    return instr2RawRR(op, r1, 0);
}

/**
 * Instr2Raw for I formats.
 */
template <chai::interpreter::Operation op>
typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] == chai::interpreter::I,
    chai::bytecode_t>::type instr2Raw(interpreter::Immidiate imm) {
    return instr2RawI(op, imm);
}

/**
 * Instr2Raw for N formats.
 */
template <chai::interpreter::Operation op>
typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] == chai::interpreter::N,
    chai::bytecode_t>::type instr2Raw() {
    return instr2RawN(op);
}

/**
 * Instr2Raw for RI formats.
 */
template <chai::interpreter::Operation op>
typename std::enable_if<chai::interpreter::OP_TO_FORMAT[op] == chai::interpreter::RI,
    chai::bytecode_t>::type instr2Raw(interpreter::RegisterId r1, interpreter::Immidiate imm) {
    return instr2RawRI(op, r1, imm);
}

} // namespace chai::utils
