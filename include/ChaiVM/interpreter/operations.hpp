#pragma once

namespace chai::interpreter {

enum Operation {
    Inv,
    Nop,
    Ret,
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

constexpr Operation opcodes2operation[] = {
    Inv,  Nop, Ret, Mov, Ldia, Ldra, Star, Add, Addi, Sub, Subi, Mul, Muli, Div,
    Divi, Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv, Inv, Inv,  Inv,  Inv,  Inv, Inv,  Inv, Inv,  Inv, Inv,  Inv,
    Inv,  Inv, Inv,
};

} // namespace chai::interpreter
