#include "ChaiVM/interpreter/executor.hpp"
#include <cassert>
#include <valarray>

namespace chai::interpreter {

#define DO_NEXT_INS()                                                          \
    Instruction newIns =                                                       \
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));              \
    (this->*handlerArr[newIns.operation])(newIns);

Executor::Executor(CodeManager *manager)
    : codeManager_(manager), regFile_(codeManager_->startPC()) {}
void Executor::run() { DO_NEXT_INS() }
const RegisterFile &Executor::getState() const & { return regFile_; }
void Executor::inv(Instruction ins) {
    throw InvalidInstruction("Invalid operation at pc: " +
                             std::to_string(regFile_.pc()));
}
void Executor::nop(Instruction ins) {
    advancePc();
    DO_NEXT_INS()
}
void Executor::ret(Instruction ins) {
    advancePc();
    return;
}
void Executor::mov(Instruction ins) {
    regFile_[ins.r2] = regFile_[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::ldia(Instruction ins) {
    regFile_.acc() = ins.immidiate;
    advancePc();
    DO_NEXT_INS()
}
void Executor::ldra(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::star(Instruction ins) {
    regFile_[ins.r1] = regFile_.acc();
    advancePc();
    DO_NEXT_INS()
}
void Executor::add(Instruction ins) {
    regFile_.acc() += regFile_[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::addi(Instruction ins) {
    regFile_.acc() += ins.immidiate;
    advancePc();
    DO_NEXT_INS()
}
void Executor::sub(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1] - regFile_.acc();
    advancePc();
    DO_NEXT_INS()
}
void Executor::subi(Instruction ins) {
    regFile_.acc() = ins.immidiate - regFile_.acc();
    advancePc();
    DO_NEXT_INS()
}
void Executor::mul(Instruction ins) {
    regFile_.acc() *= regFile_[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::muli(Instruction ins) {
    regFile_.acc() *= ins.immidiate;
    advancePc();
    DO_NEXT_INS()
}
void Executor::div(Instruction ins) {
    regFile_.acc() = regFile_[ins.r1] / regFile_.acc();
    advancePc();
    DO_NEXT_INS()
}
void Executor::divi(Instruction ins) {
    regFile_.acc() = ins.immidiate / regFile_.acc();
    advancePc();
    DO_NEXT_INS()
}
void Executor::ldiaf(Instruction ins) {
    auto immd = static_cast<double>(std::bit_cast<float>(ins.immidiate));
    regFile_.acc() = std::bit_cast<chsize_t>(immd);
    advancePc();
    DO_NEXT_INS()
}

void Executor::addf(Instruction ins) {
    double sum = std::bit_cast<double>(regFile_[ins.r1]) +
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    DO_NEXT_INS()
}
void Executor::addif(Instruction ins) {
    double sum = static_cast<double>(std::bit_cast<float>(ins.immidiate)) +
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subf(Instruction ins) {
    double dif = std::bit_cast<double>(regFile_[ins.r1]) -
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subif(Instruction ins) {
    double dif = static_cast<double>(std::bit_cast<float>(ins.immidiate)) -
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulf(Instruction ins) {
    double sum = std::bit_cast<double>(regFile_[ins.r1]) *
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulif(Instruction ins) {
    double dif = static_cast<double>(std::bit_cast<float>(ins.immidiate)) *
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divf(Instruction ins) {
    double sum = std::bit_cast<double>(regFile_[ins.r1]) /
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(sum);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divif(Instruction ins) {
    double dif = static_cast<double>(std::bit_cast<float>(ins.immidiate)) /
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(dif);
    advancePc();
    DO_NEXT_INS()
}

void Executor::icprint(Instruction ins) {
    assert(regFile_.acc() <= 0xFF);
    printf("%c", static_cast<char>(regFile_.acc()));
    advancePc();
    DO_NEXT_INS()
}

void Executor::icscani(Instruction ins) {
    int data;
    std::cin >> data;
    regFile_.acc() = data;
    advancePc();
    DO_NEXT_INS()
}

void Executor::icscanf(Instruction ins) {
    float data;
    std::cin >> data;
    regFile_.acc() = std::bit_cast<chsize_t>(static_cast<double>(data));
    advancePc();
    DO_NEXT_INS()
}

void Executor::icsqrt(Instruction ins) {
    regFile_.acc() =std::bit_cast<chsize_t> (
        sqrt (
            std::bit_cast<double>(
                regFile_.acc()
            )
        )
    );
    advancePc();
    DO_NEXT_INS()
}

void Executor::icsin(Instruction ins) {
    regFile_.acc() =std::bit_cast<chsize_t> (
        sin (
            std::bit_cast<double>(
                regFile_.acc()
            )
        )
    );
    advancePc();
    DO_NEXT_INS()
}

void Executor::iccos(Instruction ins) {
    regFile_.acc() =std::bit_cast<chsize_t> (
        cos (
            std::bit_cast<double>(
                regFile_.acc()
            )
        )
    );
    advancePc();
    DO_NEXT_INS()
}

InvalidInstruction::InvalidInstruction(const char *msg) : runtime_error(msg) {}
InvalidInstruction::InvalidInstruction(const std::string &msg)
    : runtime_error(msg) {}
const char *InvalidInstruction::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
