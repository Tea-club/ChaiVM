#include <cassert>
#include <cmath>

#include "ChaiVM/interpreter/executor.hpp"

namespace chai::interpreter {

#define DO_NEXT_INS()                                                          \
    Instruction newIns =                                                       \
        decoder::parse(codeManager_->getBytecode(regFile_.pc()));              \
    (this->*HANDLER_ARR[newIns.operation])(newIns);

Executor::Executor(CodeManager *manager)
    : codeManager_(manager), regFile_(codeManager_->startPC()) {}
void Executor::run() { DO_NEXT_INS() }
void Executor::restart() { regFile_.pc() = codeManager_->startPC(); }
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
    regFile_.acc() = codeManager_->getCnst(ins.immidiate);
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
    regFile_.acc() += codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::sub(Instruction ins) {
    regFile_.acc() -= regFile_[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::subi(Instruction ins) {
    regFile_.acc() -= codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mul(Instruction ins) {
    regFile_.acc() *= regFile_[ins.r1];
    advancePc();
    DO_NEXT_INS()
}
void Executor::muli(Instruction ins) {
    regFile_.acc() *= codeManager_->getCnst(ins.immidiate);
    advancePc();
    DO_NEXT_INS()
}
void Executor::div(Instruction ins) {
    regFile_.acc() =
        static_cast<chsize_t>(std::bit_cast<int64_t>(regFile_.acc()) /
                              static_cast<int64_t>(regFile_[ins.r1]));
    advancePc();
    DO_NEXT_INS()
}
void Executor::divi(Instruction ins) {
    regFile_.acc() = std::bit_cast<chsize_t>(
        static_cast<int64_t>(regFile_.acc()) /
        static_cast<int64_t>(codeManager_->getCnst(ins.immidiate)));
    advancePc();
    DO_NEXT_INS()
}
void Executor::ldiaf(Instruction ins) {
    double immd = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    regFile_.acc() = std::bit_cast<chsize_t>(immd);
    advancePc();
    DO_NEXT_INS()
}

void Executor::addf(Instruction ins) {
    double res = std::bit_cast<double>(regFile_[ins.r1]) +
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::addif(Instruction ins) {
    double res = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate)) +
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subf(Instruction ins) {
    double res = std::bit_cast<double>(regFile_.acc()) -
                 std::bit_cast<double>(regFile_[ins.r1]);
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::subif(Instruction ins) {
    double res = std::bit_cast<double>(regFile_.acc()) -
                 std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulf(Instruction ins) {
    double res = std::bit_cast<double>(regFile_[ins.r1]) *
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::mulif(Instruction ins) {
    double res = std::bit_cast<double>(codeManager_->getCnst(ins.immidiate)) *
                 std::bit_cast<double>(regFile_.acc());
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divf(Instruction ins) {
    double res = std::bit_cast<double>(regFile_.acc()) /
                 std::bit_cast<double>(regFile_[ins.r1]);
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::divif(Instruction ins) {
    double res = std::bit_cast<double>(regFile_.acc()) /
                 std::bit_cast<double>(codeManager_->getCnst(ins.immidiate));
    regFile_.acc() = std::bit_cast<chsize_t>(res);
    advancePc();
    DO_NEXT_INS()
}
void Executor::icprint(Instruction ins) {
    assert(regFile_.acc() <= 0xFF);
    advancePc();
    DO_NEXT_INS()
}
void Executor::icscani(Instruction ins) {
    int64_t data;
    std::cin >> data;
    regFile_.acc() = data;
    advancePc();
    DO_NEXT_INS()
}
void Executor::icscanf(Instruction ins) {
    double data;
    std::cin >> data;
    regFile_.acc() = std::bit_cast<chsize_t>(data);
    advancePc();
    DO_NEXT_INS()
}
void Executor::icsqrt(Instruction ins) {
    regFile_.acc() = std::bit_cast<chsize_t>(
        std::sqrt(std::bit_cast<double>(regFile_.acc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::icsin(Instruction ins) {
    regFile_.acc() = std::bit_cast<chsize_t>(
        std::sin(std::bit_cast<double>(regFile_.acc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::iccos(Instruction ins) {
    regFile_.acc() = std::bit_cast<chsize_t>(
        std::cos(std::bit_cast<double>(regFile_.acc())));
    advancePc();
    DO_NEXT_INS()
}
void Executor::if_icmpeq(Instruction ins) {
    if (regFile_.acc() == regFile_[ins.r1]) {
        static_assert(sizeof(Immidiate) == sizeof(int16_t));
        regFile_.pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpne(Instruction ins) {
    if (regFile_.acc() != regFile_[ins.r1]) {
        regFile_.pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpgt(Instruction ins) {
    if (regFile_.acc() > regFile_[ins.r1]) {
        regFile_.pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmpge(Instruction ins) {
    if (regFile_.acc() >= regFile_[ins.r1]) {
        regFile_.pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmplt(Instruction ins) {
    if (regFile_.acc() < regFile_[ins.r1]) {
        regFile_.pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_icmple(Instruction ins) {
    if (regFile_.acc() <= regFile_[ins.r1]) {
        regFile_.pc() += static_cast<int16_t>(ins.immidiate);
    } else {
        advancePc();
    }
    DO_NEXT_INS()
}
void Executor::if_acmpeq(Instruction ins) {
    /*
     * @todo #42:90min Implement the instruction with object ref
     *  when objects will be introduced in chai.
     */
    DO_NEXT_INS()
}
void Executor::if_acmpne(Instruction ins) {
    /*
     * @todo #42:90min Implement the instruction with object ref
     *  when chai objects will be introduced.
     */
    DO_NEXT_INS()
}
void Executor::cmpgf(Instruction ins) {
    double acc_f64 = std::bit_cast<double>(regFile_.acc());
    double r1_f64 = std::bit_cast<double>(regFile_[ins.r1]);
    regFile_.acc() = (acc_f64 < r1_f64 || acc_f64 != acc_f64 || r1_f64 != r1_f64
                          ? static_cast<chsize_t>(-1)
                          : static_cast<chsize_t>(acc_f64 != r1_f64));
    advancePc();
    DO_NEXT_INS()
}
void Executor::cmplf(Instruction ins) {
    double acc_f64 = std::bit_cast<double>(regFile_.acc());
    double r1_f64 = std::bit_cast<double>(regFile_[ins.r1]);
    regFile_.acc() = (acc_f64 > r1_f64 || acc_f64 != acc_f64 || r1_f64 != r1_f64
                          ? -1
                          : static_cast<chsize_t>(acc_f64 != r1_f64));
    advancePc();
    DO_NEXT_INS()
}
void Executor::g0t0(Instruction ins) {
    regFile_.pc() += static_cast<int16_t>(ins.immidiate);
    DO_NEXT_INS()
}

InvalidInstruction::InvalidInstruction(const char *msg) : runtime_error(msg) {}
InvalidInstruction::InvalidInstruction(const std::string &msg)
    : runtime_error(msg) {}
const char *InvalidInstruction::what() const noexcept {
    return runtime_error::what();
}

} // namespace chai::interpreter
