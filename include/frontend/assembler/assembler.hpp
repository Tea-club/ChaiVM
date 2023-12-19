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
            processInstruction();
            lex_.nextLexem();
            if (lex_.currentLexem()->type == AsmLex::IDENTIFIER &&
                OpString(
                    static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                        ->value) == chai::interpreter::Ret) {
                processInstruction();
                break;
            }
        }
    }
    void processInstruction() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        if (op == chai::interpreter::Inv) {
            throw AssembleError("Invalid instruction", lex_.lineno());
        }
        switch (opToFormat(op)) {
        case chai::interpreter::N:
            processN();
            break;
        case chai::interpreter::R:
            processR();
            break;
        case chai::interpreter::RR:
            processRR();
            break;
        case chai::interpreter::I:
            processI();
            break;
        case chai::interpreter::RI:
            processRI();
            break;
        case chai::interpreter::Unknown:
        default:
            throw AssembleError("Unknown instruction type", lex_.lineno());
            break;
        }
    }
    void processN() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        chaiFile_.addInstr(chai::utils::instr2Raw(op, 0, 0));
    }
    void processR() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        chai::interpreter::RegisterId regId = processReg();
        chaiFile_.addInstr(chai::utils::instr2Raw(op, regId, 0));
    }
    void processRR() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        chai::interpreter::RegisterId reg1Id = processReg();
        expectComma();
        chai::interpreter::RegisterId reg2Id = processReg();
        chaiFile_.addInstr(chai::utils::instr2Raw(op, reg1Id, reg2Id));
    }
    void processI() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        lex_.nextLexem();
        if (lex_.currentLexem()->type == AsmLex::INTEGER) {
            chaiFile_.addWithConst(
                op, static_cast<int64_t>(
                        static_cast<AsmLex::Int *>(lex_.currentLexem().get())
                            ->value));
        } else if (lex_.currentLexem()->type == AsmLex::FLOAT) {
            chaiFile_.addWithConst(
                op, (static_cast<AsmLex::Float *>(lex_.currentLexem().get())
                         ->value));
        } else {
            throw AssembleError("Unknown instruction type", lex_.lineno());
        }
    }
    void processRI() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        chai::interpreter::RegisterId regId = processReg();
        expectComma();
        lex_.nextLexem();
        if (lex_.currentLexem()->type == AsmLex::INTEGER) {
            chaiFile_.addInstr(chai::utils::inst2RawRI(
                op, regId,
                static_cast<int64_t>(
                    static_cast<AsmLex::Int *>(lex_.currentLexem().get())
                        ->value)));
        } else if (lex_.currentLexem()->type == AsmLex::FLOAT) {
            chaiFile_.addInstr(chai::utils::inst2RawRI(
                op, regId,
                static_cast<AsmLex::Float *>(lex_.currentLexem().get())
                    ->value));
        } else {
            throw AssembleError("Unknown instruction type", lex_.lineno());
        }
    }

    chai::interpreter::RegisterId processReg() {
        lex_.nextLexem();
        if (lex_.currentLexem()->type != AsmLex::IDENTIFIER) {
            throw AssembleError("Expected register", lex_.lineno());
        }
        return RegNameToRegId(
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
    chai::interpreter::RegisterId RegNameToRegId(std::string regName) {
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
