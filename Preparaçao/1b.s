    .equ SFR_BASE_HI,   0xBF88
    .equ TRISE,         0x6100
    .equ TRISB,         0x6040
    .equ LATE,          0x6120
    .equ PORTB,         0x6050

    .data
    .text
    .globl main

# $t0 = SFR_BASE_HI
# $t1 = TRISE
# $t2 = TRISB
# $t3 = PORTB
# $t4 = LATE
# $t5 = aux register
# $t6 = RE inverted

main:   lui $t0, SFR_BASE_HI
        lw $t1, TRISE($t0)
        lw $t2, TRISB($t0)

        andi $t1, $t1, 0xFFF0   # RE[3..0] OUTPUTS
        sw $t1, TRISE($t0)

        ori $t2, $t2, 15    # RB[3..0] INPUTS
        sw $t2, TRISB($t0)

loop:   lw $t3, PORTB($t0)
        andi $t3, $t3, 15   # READ RB[3..0]
        
        li $t6, 0

        andi $t5, $t3, 8
        srl $t5,$t5, 3
        or $t6, $t6, $t5
        
        andi $t5, $t3, 4
        srl $t5,$t5, 1
        or $t6, $t6, $t5
        
        andi $t5, $t3, 2
        sll $t5,$t5, 1
        or $t6, $t6, $t5
        
        andi $t5, $t3, 1
        sll $t5, $t5, 3
        or $t6, $t6, $t5

        sw $t6, LATE($t0)

        j loop
        jr $ra
