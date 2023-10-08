#include "ChaiVM/interpreter/executor.hpp"

namespace chai::interpreter {

Executor::Executor() : regFile_(0) {}
void Executor::execute(Instruction ins) {
    (this->*handlerArr[ins.operation])(ins);
}
const RegisterFile &Executor::getState() const & { return regFile_; }
void Executor::advancePc() { regFile_.pc() += sizeof(bytecode_t); }
void Executor::inv(Instruction ins) {
    throw InvalidInstruction("Invalid operation at pc: " +
                             std::to_string(regFile_.pc()));
}
void Executor::nop(Instruction ins) {
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::ret(Instruction ins) {
    advancePc();
    return;
}
void Executor::mov(Instruction ins) {
    regFile_[ins.r2] = regFile_[ins.r1];
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::ldia(Instruction ins) {
    regFile_.acc() = ins.immidiate;
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::ldra(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1];
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::star(Instruction ins) {
    regFile_[ins.r1] = regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::add(Instruction ins) {
    regFile_.acc() += regFile_[ins.r1];
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::addi(Instruction ins) {
    regFile_.acc() += ins.immidiate;
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::sub(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1] - regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::subi(Instruction ins) {
    regFile_.acc() = ins.immidiate - regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::mul(Instruction ins) {
    regFile_.acc() *= regFile_[ins.r1];
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::muli(Instruction ins) {
    regFile_.acc() *= ins.immidiate;
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::div(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1] / regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::divi(Instruction ins) {
    regFile_.acc() = ins.immidiate / regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_.getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}

InvalidInstruction::InvalidInstruction(const char *msg)
    : runtime_error(msg) {}
InvalidInstruction::InvalidInstruction(const std::string &msg)
    : runtime_error(msg) {}
const char *InvalidInstruction::what() const noexcept {
    return runtime_error::what();
}

}
