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

main:   lui $t0, SFR_BASE_HI
        lw $t1, TRISE($t0)
        lw $t2, TRISB($t0)

        andi $t1, $t1, 0xFFF0   # RE[3..0] OUTPUTS
        sw $t1, TRISE($t0)

        ori $t2, $t2, 15    # RB[3..0] INPUTS
        sw $t2, TRISB($t0)

loop:   lw $t3, PORTB($t0)
        andi $t3, $t3, 15   # READ RB[3..0]

        sw $t3, LATE($t0)   # RB TO RE

        j loop
        jr $ra
