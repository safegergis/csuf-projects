#Author: Safe Gergis

.data
    numbers: .byte 12, 97, 133, 82, 236
    result: .word 0
    
.text
.globl main
main:
    li $t0, 0
    li $t1, 0
    li $t2, 5
    
    la $t3, numbers
    
sum_loop:
    lb $t4, ($t3)
    add $t0, $t0, $t4
    
    addi $t3, $t3, 1
    addi $t1, $t1, 1
    
    bne $t1, $t2, sum_loop
    
    div $t0, $t2
    mflo $t0
    
    sw $t0, result
    
    li $v0, 10
    syscall
