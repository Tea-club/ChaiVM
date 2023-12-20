#pragma once

#include <sstream>
#include <utility>

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
        while (lex_.currentLexem()->type != AsmLex::EOF_) {
            processFunction();
            lex_.nextLexem();
        }
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
        }
    }
    /*
     * @todo #76:90min Add function description to assembler
     */
    /*
     * @todo #76:90min Add function to constant pool before parsing instructions (for recursion)
     */
    void processFunction() {
        expectCurrentLexem(AsmLex::FUNC, "Expected function declaration");
        expectNextLexem(AsmLex::IDENTIFIER, "Expected function name");
        std::string func_name = static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())->value;
        expectNextLexem(AsmLex::INTEGER, "Expected number of registers in function frame");
        auto num_regs = static_cast<uint8_t>(static_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
        expectNextLexem(AsmLex::INTEGER, "Expected number of function arguments");
        auto num_args = static_cast<uint8_t>(static_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
        expectNextLexem(AsmLex::OP_CURLY_BRACKET, "Expected opening curly bracket");
        std::vector<chai::bytecode_t> instrs;
        lex_.nextLexem();
        while (lex_.currentLexem()->type != AsmLex::CL_CURLY_BRACKET) {
            instrs.push_back(processInstruction());
            lex_.nextLexem();
        }
        expectCurrentLexem(AsmLex::CL_CURLY_BRACKET, "Expected closing curly bracket");
        chaiFile_.addFunction(UINT16_MAX, func_name, "V(V)", instrs, num_args, num_regs);
    }
    chai::bytecode_t processInstruction() {
        chai::interpreter::Operation op = OpString(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        if (op == chai::interpreter::Inv) {
            throw AssembleError("Invalid instruction", lex_.lineno());
        }
        if (op == chai::interpreter::Call) {
            return processCall(op);
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
    chai::bytecode_t processCall(chai::interpreter::Operation op) {
        auto val = static_cast<uint64_t>(
            static_cast<AsmLex::Int *>(lex_.nextLexem().get())->value);
        return chai::utils::instr2Raw(op, val, 0);
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
        expectNextLexem(AsmLex::COMMA, "Expected coma");
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
        expectNextLexem(AsmLex::COMMA, "Expected coma");
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
        expectNextLexem(AsmLex::IDENTIFIER, "Expected register name");
        return regNameToRegId(
            static_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
    }

    void checkError() {
        if (lex_.currentLexem()->type == AsmLex::UNKNOWN) {
            throw AssembleError("Unknown lexem", lex_.lineno());
        }
    }
    void expectNextLexem(AsmLex::LexemType type, std::string msg) {
        lex_.nextLexem();
        if (lex_.currentLexem()->type != type) {
            throw AssembleError(std::move(msg), lex_.lineno());
        }
    }
    void expectCurrentLexem(AsmLex::LexemType type, std::string msg) {
        if (lex_.currentLexem()->type != type) {
            throw AssembleError(std::move(msg), lex_.lineno());
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
