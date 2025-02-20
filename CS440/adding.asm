# Program to add 3 + 2.text

main:       

    li $t0, 3      # Load immediate value 3 into register $t0
    li $t1, 2      # Load immediate value 2 into register $t1
    add $t2, $t0, $t1 # Add the contents of $t0 and $t1, store result in $t2


    li $v0, 10     # System call code for exit
    syscall         # Exit the program