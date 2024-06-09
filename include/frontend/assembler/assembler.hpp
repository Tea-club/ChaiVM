#pragma once

#include <sstream>
#include <utility>

#include "ChaiVM/interpreter/autogen/operations.hpp"
#include "ChaiVM/utils/file-format/chai-file.hpp"
#include "ChaiVM/utils/instr2Raw.hpp"
#include "frontend/assembler/asmlex.hpp"
#include "op-with-info.hpp"

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

    void collectFunstions() {
        uint16_t func_id = 0;
        while (lex_.nextLexem()->type != AsmLex::EOF_) {
            if (lex_.currentLexem()->type == AsmLex::FUNC) {
                func_id++;
                expectNextLexem(AsmLex::IDENTIFIER, "Expected function name");
                std::string func_name = dynamic_cast<AsmLex::Identifier *>(
                        lex_.currentLexem().get()
                    )->value;
                funcsIdByName_.insert({func_name, func_id});
            }
        }
        inputFile_.seekg(0);
        lex_.yyrestart(inputFile_);
    }

    /*
     * @todo #41:90min Implement adequate processing of the main function
     */
    void assemble() {
        collectFunstions();
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
    std::unordered_map<std::string, uint16_t> funcsIdByName_;

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
        std::string func_name = dynamic_cast<AsmLex::Identifier *>(lex_.currentLexem().get())->value;
        expectNextLexem(AsmLex::INTEGER, "Expected number of registers in function frame");
        auto num_regs = static_cast<uint8_t>(dynamic_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
        expectNextLexem(AsmLex::INTEGER, "Expected number of function arguments");
        auto num_args = static_cast<uint8_t>(dynamic_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
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
        SmartOperation op = SmartOperation(
            dynamic_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
                ->value);
        if (op == chai::interpreter::Inv) {
            throw AssembleError("Invalid instruction", lex_.lineno());
        }
        if (op == chai::interpreter::Call) {
            return processCall(op);
        }
        switch (op.format()) {
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
        auto func_name = static_cast<std::string>(
            static_cast<AsmLex::String *>(lex_.nextLexem().get())->value);
        uint16_t func_id = funcsIdByName_[func_name];
        return chai::utils::instr2RawRI(op, func_id, func_id);
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
                dynamic_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstI64>(val));
            return chai::utils::instr2Raw(op, imm);
        } else if (lex_.currentLexem()->type == AsmLex::FLOAT) {
            auto val =
                dynamic_cast<AsmLex::Float *>(lex_.currentLexem().get())->value;
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstF64>(val));
            return chai::utils::instr2Raw(op, imm);
        } else if (lex_.currentLexem()->type == AsmLex::STRING) {
            std::string str =
                dynamic_cast<AsmLex::String *>(lex_.currentLexem().get())->value;
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
                dynamic_cast<AsmLex::Int *>(lex_.currentLexem().get())->value);
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstI64>(val));
            return chai::utils::instr2RawRI(op, regId, imm);
        } else if (lex_.currentLexem()->type == AsmLex::FLOAT) {
            auto val =
                dynamic_cast<AsmLex::Float *>(lex_.currentLexem().get())->value;
            auto imm = chaiFile_.addConst(
                std::make_unique<chai::utils::fileformat::ConstI64>(val));
            return chai::utils::instr2RawRI(op, regId, imm);
        } else if (lex_.currentLexem()->type == AsmLex::STRING) {
            std::string str =
                dynamic_cast<AsmLex::String *>(lex_.currentLexem().get())->value;
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
            dynamic_cast<AsmLex::Identifier *>(lex_.currentLexem().get())
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
    chai::interpreter::RegisterId regNameToRegId(std::string regName) {
        chai::interpreter::RegisterId regId;
        if (regName.length() > 1 && regName[0] == 'r') {
            std::string digits = regName.substr(1);
            regId = std::stoi(digits);
            std::istringstream iss(digits);
        } else {
            throw AssembleError("Invalid register format", lex_.lineno());
        }
        return regId;
    }
};

} // namespace front::assembler
