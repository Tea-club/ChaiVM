#include "ChaiVM/interpreter/executor.hpp"

namespace chai::interpreter {

Executor::Executor(CodeManager *manager)
    : codeManager_(manager), regFile_(codeManager_->startPC()) {}
void Executor::run() {
    const Instruction first =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[first.operation])(first);
}
const RegisterFile &Executor::getState() const & { return regFile_; }
void Executor::inv(Instruction ins) {
    throw InvalidInstruction("Invalid operation at pc: " +
                             std::to_string(regFile_.pc()));
}
void Executor::nop(Instruction ins) {
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
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
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::ldia(Instruction ins) {
    regFile_.acc() = ins.immidiate;
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::ldra(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1];
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::star(Instruction ins) {
    regFile_[ins.r1] = regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::add(Instruction ins) {
    regFile_.acc() += regFile_[ins.r1];
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::addi(Instruction ins) {
    regFile_.acc() += ins.immidiate;
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::sub(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1] - regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::subi(Instruction ins) {
    regFile_.acc() = ins.immidiate - regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::mul(Instruction ins) {
    regFile_.acc() *= regFile_[ins.r1];
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::muli(Instruction ins) {
    regFile_.acc() *= ins.immidiate;
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::div(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1] / regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::divi(Instruction ins) {
    regFile_.acc() = ins.immidiate / regFile_.acc();
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::ldiaf(Instruction ins) {
    double immd = static_cast<double>(std::bit_cast<float>(ins.immidiate));
    regFile_.acc() = std::bit_cast<chsize_t>(immd);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::addf(Instruction ins) {
    double sum = std::bit_cast<double>(regFile_[ins.r1]) +
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::addif(Instruction ins) {
    double sum = static_cast<double>(std::bit_cast<float>(ins.immidiate)) +
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::subf(Instruction ins) {
    double dif = std::bit_cast<double>(regFile_[ins.r1]) -
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::subif(Instruction ins) {
    double dif = static_cast<double>(std::bit_cast<float>(ins.immidiate)) -
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::mulf(Instruction ins) {
    double sum = std::bit_cast<double>(regFile_[ins.r1]) *
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::mulif(Instruction ins) {
    double dif = static_cast<double>(std::bit_cast<float>(ins.immidiate)) *
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::divf(Instruction ins) {
    double sum = std::bit_cast<double>(regFile_[ins.r1]) /
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}
void Executor::divif(Instruction ins) {
    double dif = static_cast<double>(std::bit_cast<float>(ins.immidiate)) /
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    Instruction newIns =
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));
    (this->*handlerArr[newIns.operation])(newIns);
}

InvalidInstruction::InvalidInstruction(const char *msg) : runtime_error(msg) {}
InvalidInstruction::InvalidInstruction(const std::string &msg)
    : runtime_error(msg) {}
const char *InvalidInstruction::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
