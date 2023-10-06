# ChaiVM - Virtual machine for chai bytecode.
## How to build:
```shell
$ make build
```
## How to use
```shell
$ build/chai <app.chai>
```
### Chai bytecode currently is just a Sequence of instructions.
For instance, there is disassembler of simple app.chai
```
Mov 6 r2
Mov 8 r3
Ldra r3
Mul r2
Print acc
Mov o ro
Ret;
```
Where Ret returns exit code `r0`.
This script compiled to chai-bytecode should print "0", because '0' == 48.
