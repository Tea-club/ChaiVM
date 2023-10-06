#ifndef CHAIVM_EXECUTOR_HPP
#define CHAIVM_EXECUTOR_HPP

#include "decoder.hpp"
#include "reg-file.hpp"

namespace chai::interpreter {

class Executor {
public:
    using Handler = void (Executor::*)(const Instruction &);

    void execute(const Instruction &ins);
    const RegisterFile &getState() const &;
    void advancePc();

    void nop(const Instruction &ins);
    void ret(const Instruction &ins);

    static constexpr Handler handlerArr[] = {&Executor::nop, &Executor::ret};

private:
    RegisterFile regFile_;
};

} // namespace chai::interpreter

#endif // CHAIVM_EXECUTOR_HPP
