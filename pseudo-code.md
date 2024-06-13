```
    /**
     * Init foo() function.
     *
     * input: acc() = N, R11 = M
     * R10 = N                         // R10 = N, R11 = M
     * Ldra r11
     * NewRefArray Foo
     * StarRef R9                       // R9 = arr
     * OUTER = null                     // OUTER == R8
     * ITER = 1                         // ITER = R7
     * start_                           // start loop
     * acc = O1 = new Foo               // O1 = R6
     * setField(ITER, 0); Star O1
     * acc = ITER; MODI three
     * if_icmpne R0 (offset second_if_)
     * R5 = ITER % M - 1
     * R9[R5] = O1
     * second_if_:
     * acc = O2 = new Bar               // O2 = R4
     * setfield (ITER, 0)
     * Star O2
     * ldra ITER
     * MODI five
     * if_cmpne R0 +3
     * acc = O1; SetField(O2, 8)
     * StarRef O1
     * LdraRef R9
     * Call dump
     * acc = ITER; acc++; Star ITER
     * if_icmple R1 -100 (go to start)
     * Ret
     */
```