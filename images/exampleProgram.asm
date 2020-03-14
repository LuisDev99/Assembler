//Para compilar este programa: ./assembler exampleProgram.asm 
//Esto generara un archivo .hex, abrirlo en la rom y correr el circuito
//res deberia de ser igual a 4 al final  

    li $r0, 0  //Cargar 0 al registro r0
    li $r1, 15 //n
    li $r2, 0  //count 
    li $r3, 1  //Constante 1
WHILE:
    beq $r1, $r0, RETURNCOUNT 
    and $r4, $r1, $r3 //n & 1
    add $r2, $r2, $r4 //count = count + (n&1)
    srl $r1, $r1, $r3 //n >>= 1 (n = n >> 1)
    jmp WHILE

RETURNCOUNT:
    //Primero imprimir un mensaje a la terminal, asi es como se hace:
    li $r7, 'C'
    li $r7, 'o'
    li $r7, 'n'
    li $r7, 't'
    li $r7, 'a'
    li $r7, 'r'
    li $r7, ' '
    li $r7, 'B'
    li $r7, 'i'
    li $r7, 't'
    li $r7, 's'
    li $r7, ' '
    li $r7, 'e'
    li $r7, 'n'
    li $r7, 'c'
    li $r7, 'e'
    li $r7, 'n'
    li $r7, 'd'
    li $r7, 'i'
    li $r7, 'd'
    li $r7, 'o'
    li $r7, 's'
    li $r7, ' '
    add $r7, $r0, $r2  //r7 = count
    looper: jmp looper //saltar otra vez al mismo lugar para que $r7 se quede con la respuesta
