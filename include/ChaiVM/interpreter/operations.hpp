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
    IcPrint,
    IcScani,
    IcScanf,
    IcSqrt,
    IcSin,
    IcCos,
};

constexpr Operation opcodes2operation[] = {
    Inv, Nop,  Ret, Mov,  Ldia,    Ldra,    Star,    Add,    Addi,  Sub,   Subi,
    Mul, Muli, Div, Divi, IcPrint, IcScani, IcScanf, IcSqrt, IcSin, IcCos, Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,  Inv, Inv,  Inv,     Inv,     Inv,     Inv,    Inv,   Inv,   Inv,
    Inv, Inv,
};

} // namespace chai::interpreter
