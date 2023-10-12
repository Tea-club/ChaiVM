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
    Ldiaf,
    Addf,
    Addif,
    Subf,
    Subif,
    Mulf,
    Mulif,
    Divf,
    Divif,
};

constexpr Operation opcodes2operation[] = {
    Inv,  Nop, Ret,  Mov,   Ldia, Ldra,  Star, Add,   Addi, Sub,   Subi, Mul,
    Muli, Div, Divi, Ldiaf, Addf, Addif, Subf, Subif, Mulf, Mulif, Divf, Divif,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,   Inv,  Inv,
    Inv,  Inv, Inv,
};
;

} // namespace chai::interpreter
