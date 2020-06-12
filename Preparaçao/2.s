    .equ SFR_BASE_HI,       0xBF88
    .equ TRISE,             0x6100
    .equ TRISB,             0x6040
    .equ LATE,              0x6120
    .equ PORTB,             0x6050
    .equ INKEY,             1
    .equ READ_CORE_TIMER,   11
    .equ RESET_CORE_TIMER,  12

    .data
    .text
    .globl main

# $t0 = SFR_BASE_HI
# $t1 = TRISE
# $t4 = LATE
# $t5 = return value

main:   lui $t0, SFR_BASE_HI
        lw $t1, TRISE($t0)
        lw $t2, TRISB($t0)

        andi $t1, $t1, 0xFFF0   # RE[3..0] OUTPUTS
        sw $t1, TRISE($t0)

        li $t4, 0  
        sw $t4, LATE($t0)    # RE[3..0] = 0
        li $t5, 0            # char c;

loop:   li $v0, INKEY
        syscall
        move $t5, $v0

b0:     bne $t5, '0', b1
        li $t4, 1
        sw $t4, LATE($t0)
        li $t5, 0

b1:     bne $t5, '1', b2
        li $t4, 2
        sw $t4, LATE($t0)
        li $t5, 0

b2:     bne $t5, '2', b3
        li $t4, 4
        sw $t4, LATE($t0)
        li $t5, 0

b3:     bne $t5, '3', else
        li $t4, 8
        sw $t4, LATE($t0)
        li $t5, 0

else:   beq $t5, 0, end
        li $t4, 15
        sw $t4, LATE($t0)

        subu $sp, $sp, 16
        sw $ra, 0($sp)
        sw $t0, 4($sp)
        sw $t4, 8($sp)
        sw $t5, 12($sp)

        li $a0, 1000
        jal delay

        lw $t5, 12($sp)
        lw $t4, 8($sp)
        lw $t0, 4($sp)
        lw $ra, 0($sp)
        addiu $sp, $sp, 16

        li $t4, 0
        sw $t4, LATE($t0)

end:    j loop
        jr $ra

delay:  move $t0, $a0               # ms = 0;
dfor:   ble $t0, 0, dexit           # for(;ms>0;ms--){
        li $v0, RESET_CORE_TIMER    # 
        syscall                     # resetCoreTimer();
dloop:  li $v0, READ_CORE_TIMER     #
        syscall                     #
        blt $v0,20000,dloop         # while(readCoreTimer() < 20000);
        subu $t0, $t0, 1            # //ms--;
        j dfor                      #
dexit:  jr $ra                      # }
