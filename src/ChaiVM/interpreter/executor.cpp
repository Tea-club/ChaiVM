#include "ChaiVM/interpreter/executor.hpp"

void chai::interpreter::Executor::execute(
    const chai::interpreter::Instruction &ins) {
    (this->*handlerArr[ins.operation])(ins);
}
const chai::interpreter::RegisterFile &
chai::interpreter::Executor::getState() const & {
    return regFile_;
}
void chai::interpreter::Executor::advancePc() {
    regFile_.setPc(regFile_.getPc() + 4);
}
void chai::interpreter::Executor::nop(
    const chai::interpreter::Instruction &ins) {
    advancePc();
    Instruction newIns = decoder_.decode(regFile_.getPc());
    (this->*handlerArr[ins.operation])(newIns);
}
void chai::interpreter::Executor::ret(
    const chai::interpreter::Instruction &ins) {
    advancePc();
    return;
}
