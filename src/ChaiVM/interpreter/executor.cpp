#include "ChaiVM/interpreter/executor.hpp"

using namespace chai::interpreter;

void Executor::execute(Instruction ins) {
    (this->*handlerArr[ins.operation])(ins);
}
const RegisterFile &Executor::getState() const & { return regFile_; }
void Executor::advancePc() { regFile_.setPc(regFile_.getPc() + 4); }
void Executor::nop(Instruction ins) {
    advancePc();
    Instruction newIns = decoder_.decode(regFile_.getPc());
    (this->*handlerArr[ins.operation])(newIns);
}
void Executor::ret(Instruction ins) {
    advancePc();
    return;
}
