# MIPS Assembly program for a bitwise OR operation.text

main:

    li $t0, 0x1A    # Load hexadecimal value 1A (26 decimal) into $t0
    ori $t1, $t0, 0x25  # OR $t0 with 0x25 (37 decimal), store in $t1

    li $v0, 10     # System call code for exit
    syscall         # Exit the program