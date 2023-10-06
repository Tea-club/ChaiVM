#ifndef CHAIVM_EXECUTOR_HPP
#define CHAIVM_EXECUTOR_HPP

#include "decoder.hpp"
#include "reg-file.hpp"

namespace chai::interpreter {

class Executor {
public:
    using Handler = void (Executor::*)(Instruction);

    void execute(Instruction ins);
    const RegisterFile &getState() const &;
    void advancePc();

    void nop(Instruction ins);
    void ret(Instruction ins);

    static constexpr Handler handlerArr[] = {&Executor::nop, &Executor::ret};

private:
    RegisterFile regFile_;
    Decoder decoder_;
};

} // namespace chai::interpreter

#endif // CHAIVM_EXECUTOR_HPP
