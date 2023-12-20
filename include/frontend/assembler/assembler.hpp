#pragma once

#include <sstream>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"
#include "frontend/assembler/asmlex.hpp"
#include "op-string.hpp"

namespace front::assembler {

class AssembleError : public std::runtime_error {
public:
    AssembleError(std::string description, size_t line)
        : std::runtime_error("Assemble error at line " + std::to_string(line) +
                             ": " + description) {}
};

class Assembler final {
public:
    Assembler(const std::filesystem::path &inPath,
              const std::filesystem::path &outPath)
        : inputFile_(inPath, std::ios::in), outPath_(outPath) {
        lex_.switch_streams(&inputFile_);
    }

    /*
     * @todo #41:90min Implement adequate processing of the main function
     */
    void assemble() {
        processMain();
        chaiFile_.toFile(outPath_);
    }

private:
    AsmLex lex_;
    chai::utils::fileformat::ChaiFile chaiFile_;
    std::ifstream inputFile_;
    std::filesystem::path outPath_;

    /*
     * @todo #41:90min Refactor this function. Or maybe it is better to kill
     * myself?
     */
    void processMain() {
        lex_.nextLexem();
        checkError();
        while (lex_.currentLexem()->type == AsmLex::IDENTIFIER) {
            checkError();
            chaiFile_.addInstr(processInstruction());
            lex_.nextLexem();
            if (lex_.currentLexem()->type == AsmLex::IDENTIFIER &&
                OpString(
                    static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                        ->value) == chai::interpreter::Ret) {
                chaiFile_.addInstr(processInstruction());
                break;
            }
        }
    }
    chai::bytecode_t processInstruction() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        if (op == chai::interpreter::Inv) {
            throw AssembleError("Invalid instruction", lex_.lineno());
        }
        switch (opToFormat(op)) {
        case chai::interpreter::N:
            return processN(op);
        case chai::interpreter::R:
            return processR(op);
        case chai::interpreter::RR:
            return processRR(op);
        case chai::interpreter::I:
            return processI(op);
        case chai::interpreter::RI:
            return processRI(op);
        case chai::interpreter::Unknown:
        default:
            throw AssembleError(
                "Unknown instruction type in processInstruction",
                lex_.lineno());
            break;
        }
    }
    chai::bytecode_t processN(chai::interpreter::Operation op) {
        return chai::utils::instr2Raw(op, 0, 0);
    }
    chai::bytecode_t processR(chai::interpreter::Operation op) {
        chai::interpreter::RegisterId regId = processReg();
        return chai::utils::instr2Raw(op, regId, 0);
    }
    chai::bytecode_t processRR(chai::interpreter::Operation op) {
        chai::interpreter::RegisterId reg1Id = processReg();
        expectComma();
        chai::interpreter::RegisterId reg2Id = processReg();
        return chai::utils::instr2Raw(op, reg1Id, reg2Id);
    }
    chai::bytecode_t processI(chai::interpreter::Operation op) {
        lex_.nextLexem();
        if (lex_.currentLexem()->type == AsmLex::INTEGER) {
            auto val = static_cast<int64_t>(
                static_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstI64>(val));
            return chai::utils::instr2Raw(op, imm);
        } else if (lex_.currentLexem()->type == AsmLex::FLOAT) {
            auto val =
                static_cast<AsmLex::Float *>(lex_.currentLexem().get())->value;
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstI64>(val));
            return chai::utils::instr2Raw(op, imm);
        } else if (lex_.currentLexem()->type == AsmLex::STRING) {
            std::string str =
                static_cast<AsmLex::String *>(lex_.currentLexem().get())->value;
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstRawStr>(str));
            return chai::utils::instr2Raw(op, imm);
        } else {
            throw AssembleError("Unknown instruction type in ProcessI",
                                lex_.lineno());
        }
    }
    chai::bytecode_t processRI(chai::interpreter::Operation op) {
        chai::interpreter::RegisterId regId = processReg();
        expectComma();
        lex_.nextLexem();
        if (lex_.currentLexem()->type == AsmLex::INTEGER) {
            auto val = static_cast<int64_t>(
                static_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstI64>(val));
            return chai::utils::instr2RawRI(op, regId, imm);
        } else if (lex_.currentLexem()->type == AsmLex::FLOAT) {
            auto val =
                static_cast<AsmLex::Float *>(lex_.currentLexem().get())->value;
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstI64>(val));
            return chai::utils::instr2RawRI(op, regId, imm);
        } else if (lex_.currentLexem()->type == AsmLex::STRING) {
            std::string str =
                static_cast<AsmLex::String *>(lex_.currentLexem().get())->value;
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstRawStr>(str));
            return chai::utils::instr2RawRI(op, regId, imm);
        } else {
            throw AssembleError("Unknown instruction type in processRI",
                                lex_.lineno());
        }
    }

    chai::interpreter::RegisterId processReg() {
        lex_.nextLexem();
        if (lex_.currentLexem()->type != AsmLex::IDENTIFIER) {
            throw AssembleError("Expected register", lex_.lineno());
        }
        return regNameToRegId(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
    }

    void checkError() {
        if (lex_.currentLexem()->type == AsmLex::UNKNOWN) {
            throw AssembleError("Unknown lexem", lex_.lineno());
        }
    }
    void expectComma() {
        lex_.nextLexem();
        if (lex_.currentLexem()->type != AsmLex::COMMA) {
            throw AssembleError("Expected comma", lex_.lineno());
        }
    }
    /*
     * @todo #41:90min Implement it in OpString class. Also rename OpString
     * class
     */
    chai::interpreter::OperationFormat
    opToFormat(chai::interpreter::Operation op) {
        return chai::interpreter::OP_TO_FORMAT[op];
    }
    chai::interpreter::RegisterId regNameToRegId(std::string regName) {
        chai::interpreter::RegisterId regId;
        if (regName.length() > 1 && regName[0] == 'r') {
            std::string digits = regName.substr(1);
            std::istringstream iss(digits);
            if (!(iss >> regId)) {
                throw AssembleError("Invalid register number", lex_.lineno());
            }
        } else {
            throw AssembleError("Invalid register format", lex_.lineno());
        }
        return regId;
    }
};

} // namespace front::assembler
