#ifndef CHAIVM_OPERATIONS_HPP
#define CHAIVM_OPERATIONS_HPP

namespace chai::interpreter {

enum Operation {
    Inv,
    Nop,
    Mov,
    Ldia,
    Ldra,
    Star,
    Add,
    Addi,
    Sub,
    Subi,
    Mul,
    Muli,
    Div,
    Divi,
};

Operation opcodes2operation[] = { Inv, Nop, Mov, Ldia, Ldra, Star, Add, Addi, Sub, Subi, Mul, Muli, Div, Divi, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, Inv, }

}

#endif //CHAIVM_OPERATIONS_HPP
