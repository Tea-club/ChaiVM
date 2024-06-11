# ChaiVM - Virtual machine for chai bytecode.
You can read more about chai isa [here](ISA.md) 
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
fn main 8 0 {
  Ldia 6
  Star r2
  Ldia 8
  Star r3
  Ldra r3
  Mul r2
  Printc acc
  Mov 0 r0
  Ret
}
```
Where Ret returns exit code `r0`. "main" is name of the starting function, `8` is number of registers in it, `0` is number of arguments passing into the function. 
This script compiled to chai-bytecode should print "0", because '0' == 48.
## Chai file format
Except for classes, our file format is similar to jvm ClassFile Structure. Its content is the following:
- Constants count (`imm`)
- Constant[]
- klass_info count (`imm`)
- klass_info[]
- func_info count (`imm`)
- func_info[]

Where `imm` is 2 bytes.

`Constant` has a 1 byte tag that specifies its type(ConstI64, or ConstFuncNameAndType and so on) and the payload then.

`klass_info` structure:
```
klass_info {
  imm ConstRawStr              // Klass name index in constant pool
  u1 nFields                   // Count of fields
  field_info[]                 // field_infos array
}

field_info {
  imm ConstRawStr              // Field name index in constant pool
  u1 type                      // 0 if primitive, some class otherwise
  union {
    u1 tag                     // (first byte 0, second determines) 1 for i64, 2 for f64
    imm klassNum               // Klass number, counting only klasses from top to bottom
  }
}
```
`func_info` structure:
```
func_info {
  imm accessFlagsCount
  imm ConstFuncNameAndType
  imm attsCount                // Count of attributes. Now only Code attribute exists
  imm attNameIndex             // Id of attribute, Now can be arbitrary
  imm attLen                   // Size of the Code atribute in bytes 
  u1 maxRegisters              // Number of registers used in function
  u1 nargs                     // Number of arguments to pass
  u4 codeLen                   // Size of code(instructions only) in bytes
  u1[] code                    // Instructions, byte array.
}         
```

## Strings
All strings are contained in CodeManager.stringPool_. Some of them initialized with starting of VM but you also can create own string in runtime via `StringConcat` or `StringSlice` instructions. 