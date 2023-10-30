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
    IcPrint,
    IcScani,
    IcScanf,
    IcSqrt,
    IcSin,
    IcCos,

};

} // namespace chai::interpreter
