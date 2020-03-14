li  $r0, 0
li  $r1, 7 //n
li  $r2, 0 //a
li  $r3, 1 //b
li  $r4, 0 //c
beq $r1, $r0, RETURNA
li  $r5, 2 //i

    slt $r6, $r5, $r1 // i < n
    beq $r6, $r0, EQUALS
    jmp INSIDEFOR
    EQUALS: beq $r5, $r1, INSIDEFOR
    jmp RETURNB
INSIDEFOR:
    add $r4, $r2, $r3 // c = a+b 
    add $r2, $r0, $r3 // a = b 
    add $r3, $r0, $r4 // b = c
    li  $r6, 1        // constant
    add $r5, $r5, $r6 // i++
    jmp CONDITION
RETURNA:
    li  $r7, 'D'
    li  $r7, 'O'
    li  $r7, 'N'
    li  $r7, 'E'
    add $r7, $r0, $r2 // return a
    LK: jmp LK //Loop forever
RETURNB:
    li  $r7, 'D'
    li  $r7, 'O'
    li  $r7, 'N'
    li  $r7, 'E'
    li  $r7, 0
    add $r7, $r0, $r3 // return b 
    LJ: jmp LJ