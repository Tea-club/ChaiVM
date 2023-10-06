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
Ldia 6
Star r2
Ldia 8
Star r3
Ldra r3
Mul r2
Printc acc
Mov o r0
Ret;
```
Where Ret returns exit code `r0`.
This script compiled to chai-bytecode should print "0", because '0' == 48.
