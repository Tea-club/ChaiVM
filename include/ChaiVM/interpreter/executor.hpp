#pragma once

#include <bit>

#include "code-manager.hpp"
#include "decoder.hpp"
#include "reg-file.hpp"

namespace chai::interpreter {

class Executor {
public:
    using Handler = void (Executor::*)(Instruction);

    Executor(CodeManager *manager);
    void run();
    void restart();
    const RegisterFile &getState() const &;

private:
    inline void advancePc();
    void inv(Instruction ins);
    void nop(Instruction ins);
    void ret(Instruction ins);
    void mov(Instruction ins);
    void ldia(Instruction ins);
    void ldra(Instruction ins);
    void star(Instruction ins);
    // @todo #8:90m/DEV Check arithmetic instruction on overflow.
    void add(Instruction ins);
    void addi(Instruction ins);
    void sub(Instruction ins);
    void subi(Instruction ins);
    void mul(Instruction ins);
    void muli(Instruction ins);
    void div(Instruction ins);
    void divi(Instruction ins);
    void ldiaf(Instruction ins);
    void addf(Instruction ins);
    void addif(Instruction ins);
    void subf(Instruction ins);
    void subif(Instruction ins);
    void mulf(Instruction ins);
    void mulif(Instruction ins);
    void divf(Instruction ins);
    void divif(Instruction ins);
    void icprint(Instruction ins);
    void icscani(Instruction ins);
    void icscanf(Instruction ins);
    void icsqrt(Instruction ins);
    void icsin(Instruction ins);
    void iccos(Instruction ins);

    static constexpr Handler handlerArr[] = {
        &Executor::inv,     &Executor::nop,     &Executor::ret,
        &Executor::mov,     &Executor::ldia,    &Executor::ldra,
        &Executor::star,    &Executor::add,     &Executor::addi,
        &Executor::sub,     &Executor::subi,    &Executor::mul,
        &Executor::muli,    &Executor::div,     &Executor::divi,
        &Executor::ldiaf,   &Executor::addf,    &Executor::addif,
        &Executor::subf,    &Executor::subif,   &Executor::mulf,
        &Executor::mulif,   &Executor::divf,    &Executor::divif,
        &Executor::icprint, &Executor::icscani, &Executor::icscanf,
        &Executor::icsqrt,  &Executor::icsin,   &Executor::iccos};

private:
    CodeManager *codeManager_;
    RegisterFile regFile_;
};

inline void Executor::advancePc() { regFile_.pc() += sizeof(bytecode_t); }

/**
 * @todo #8:30m>/DEV make this exception to take RegisterFile and return it's
 * dump in a string via `what()`
 */
class InvalidInstruction : public std::runtime_error {
public:
    InvalidInstruction(char const *msg);
    InvalidInstruction(const std::string &msg);
    const char *what() const noexcept override;
};

} // namespace chai::interpreter
