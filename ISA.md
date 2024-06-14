There is ChaiVM's accumulator(getAcc) based ISA.

Notation: `imm` is raw value, `[imm]` is value from constant pool by number imm.

| Operation | Format | Description |
|:---------:|:------:|:------------|
| Nop | N | Does nothing |
| Ret | N | Returns a value via getAcc |
| Mov | RR | i64, moves value from ri to rj |
| Ldia | I | i64, loads constant [i] to getAcc |
| Ldra | R | i64, loads the R1 to getAcc |
| Star | R | i64, copies val of getAcc into register ri |
| Add | R | i64, Adds R1 to getAcc |
| Addi | I | i64, Adds [imm] to getAcc |
| Sub | R | i64, sub R1 from getAcc |
| Subi | I | i64, sub [imm] from getAcc |
| Mul | R | i64, mul getAcc by R1 |
| Muli | I | i64, mul getAcc by [imm] |
| Div | R | i64, divides getAcc by R1 |
| Divi | I | i64, divides getAcc by [imm] |
| Modi | I | i64, getAcc = getAcc % [imm] |
| Ldiaf | I | f64, loads constant [i] to getAcc |
| Addf | R | f64, Adds R1 to getAcc |
| Addif | I | f64, Adds [imm] to getAcc |
| Subf | R | f64, sub R1 from getAcc |
| Subif | I | i64, sub [imm] from getAcc |
| Mulf | R | f64, mul getAcc by R1 |
| Mulif | I | f64, mul getAcc by [imm] |
| Divf | R | f64, divides getAcc by R1 |
| Divif | I | f64, divides getAcc by [imm] |
| IcPrint | N | Intrinsic. Prints value of getAcc |
| IcScani | N | Intrinsic. Scans i64 from stdin into getAcc |
| IcScanf | N | Intrinsic. Scans f64 from stdin into getAcc |
| IcSqrt | N | f64, Intrinsic. Calculates sqrt of getAcc. |
| IcSin | N | f64, Intrinsic. Calculates sin of getAcc. |
| IcCos | N | f64, Intrinsic. Calculates cos of getAcc. |
| If_icmpeq | RI | i64, if getAcc == r1 then branch to instruction at offset(instructions) imm otherwise just next instr |
| If_icmpne | RI | i64, if getAcc != r1 then branch to instruction at offset(instructions) imm otherwise just next instr |
| If_icmpgt | RI | i64, if getAcc > r1 then branch to instruction at offset(instructions) imm otherwise just next instr |
| If_icmpge | RI | i64, if getAcc >= r1 then branch to instruction at offset(instructions) imm otherwise just next instr |
| If_icmplt | RI | i64, if getAcc < r1 then branch to instruction at offset(instructions) imm otherwise just next instr |
| If_icmple | RI | i64, if getAcc <= r1 then branch to instruction at offset(instructions) imm otherwise just next instr |
| If_acmpeq | RI | ref, if references are equal, branch to instruction at offset(instructions) imm |
| If_acmpne | RI | ref, if references are not equal, branch to instruction at offset(instructions) imm |
| If_null | I | ref, if reference in getAcc is null, branch to instruction at offset(instructions) imm |
| Сmpgf | R | f64 -> i64, compare getAcc with r1. Acc became 1 i64 if greater than r1, 0 if equal, otherwise -1 |
| Cmplf | R | f64 -> i64, compare getAcc with r1. Acc became 1 i64 if less than r1, 0 if equal, otherwise -1 |
| Goto | I | Goes to another instruction at branchoffset(instructions) imm |
| Call | I | Calls function [imm]. Imm is reference to function in constant pool named constant_func_name_and_type. |
| NewI64Array | N | Allocates array of type i64 with number of elements from getAcc register. A reference to this new array is stored in getAcc. |
| GetI64FromArr | R | Gets i64 from i64 array in getAcc and puts it to acc. |
| SetI64InArr | RR | Sets i64 value to i64 array in acc and puts it to getAcc. |
| NewF64Array | N | Allocates array of type f64 with number of elements from getAcc register. A reference to this new array is stored in getAcc. |
| GetF64FromArr | R | Gets f64 from f64 array in getAcc and puts it to acc. |
| SetF64InArr | RR | Sets f64 value to f64 array in getAcc and puts it to acc. |
| NewRefArray | N | Allocates array of objects with number of elements from getAcc register. A reference to this new array is stored in getAcc. Value of every object is null. |
| GetRefFromArr | R | Gets object by index in r1 from object array in getAcc and puts it to acc. |
| SetRefInArr | RR | Sets ref value in r2 by index in r1 to object array in getAcc and puts it to acc. |
| StringPrint | N | Prints String in getAcc |
| StringConcat | R | Create new String, concatenating String in getAcc and String in r1. Result in getAcc. |
| StringLen | N | Puts len of String to getAcc |
| StringSlice | RR | Create new String slicing String placed in getAcc. r1 is start and r2 is finish of slicing |
| AllocRef | I | Ref, Creates object of klass [imm], leaves ref in getAcc |
| MovRef | RR | Ref, copies ref from ri to rj |
| LdraRef | R | Ref, copies ref of the R1 to getAcc |
| StarRef | R | Ref, copies ref of getAcc into register ri |
| GetField | I | Ref, load value of field to getAcc. imm is offset of field (starting from all fields) Value can be ref, array or String |
| SetField | RI | Ref, set value of register to field imm of object in getAcc. Value should be consistent to field signature. There are no checks in runtime. |

To generate this file use the following python script:
```shell
$ python3 tools/isa-spec.py
```