There is ChaiVM's accumulator(acc) based ISA.

| Operation | Format | Description |
|:---------:|:------:|:------------|
| Nop | N | Does nothing |
| Ret | N | Returns a value via acc |
| Mov | RR | i64, moves value from ri to rj |
| Ldia | R | i64, loads constant [i] to acc |
| Ldra | R | i64, loads the R1 to acc |
| Star | R | i64, copies val of acc into register ri |
| Add | R | i64, Adds R1 to acc |
| Addi | I | i64, Adds [imm] to acc |
| Sub | R | i64, sub R1 from acc |
| Subi | I | i64, sub [imm] from acc |
| Mul | R | i64, mul acc by R1 |
| Muli | I | i64, mul acc by [imm] |
| Div | R | i64, divides acc by R1 |
| Divi | I | i64, divides acc by [imm] |
| Ldiaf | R | f64, loads constant [i] to acc |
| Addf | R | f64, Adds R1 to acc |
| Addif | I | f64, Adds [imm] to acc |
| Subf | R | f64, sub R1 from acc |
| Subif | I | i64, sub [imm] from acc |
| Mulf | R | f64, mul acc by R1 |
| Mulif | I | f64, mul acc by [imm] |
| Divf | R | f64, divides acc by R1 |
| Divif | I | f64, divides acc by [imm] |
| IcPrint | N | Intrinsic. Prints char, containing by the first byte of acc |
| IcScani | N | Intrinsic. Scans i64 from stdin |
| IcScanf | N | Intrinsic. Scans f64 from stdin |
| IcSqrt | N | f64, Intrinsic. Calculates sqrt of acc. |
| IcSin | N | f64, Intrinsic. Calculates sin of acc. |
| IcCos | N | f64, Intrinsic. Calculates cos of acc. |
| If_icmpeq | N | i64, if acc == r1 then branch to instruction at offset [imm] otherwise just next instr |
| If_icmpne | RI | i64, if acc != r1 then branch to instruction at offset [imm] otherwise just next instr |
| If_icmpgt | RI | i64, if acc > r1 then branch to instruction at offset [imm] otherwise just next instr |
| If_icmpge | RI | i64, if acc >= r1 then branch to instruction at offset [imm] otherwise just next instr |
| If_icmplt | RI | i64, if acc < r1 then branch to instruction at offset [imm] otherwise just next instr |
| If_icmple | RI | i64, if acc <= r1 then branch to instruction at offset [imm] otherwise just next instr |
| If_acmpeq | RI | ref, if references are equal, branch to instruction at offset [imm] |
| If_acmpne | RI | ref, if references are not equal, branch to instruction at offset [imm] |
| Сmpgf | R | f64 -> i64, compare acc with r1. Acc became 1 i64 if greater than r1, 0 if equal, otherwise -1 |
| Cmplf | R | f64 -> i64, compare acc with r1. Acc became 1 i64 if less than r1, 0 if equal, otherwise -1 |
| Goto | I | Goes to another instruction at branchoffset [imm] |
| Call | I | Calls function [imm]. Imm is reference to function in constant pool named constant_func_name_and_type. |

To generate this file use the following python script:
```shell
$ python3 tools/isa-spec.py
```