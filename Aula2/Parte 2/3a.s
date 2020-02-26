	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ PUT_CHAR, 3
	.equ PRINT_INT, 6

	.data
	.text
	.globl main

main:	la $a0, ' '
	li $v0, PUT_CHAR
	syscall
	
	lui $t1, 0xBF88
	lw $t2, 0x6050($t1)
	and $t2,$t2,0x000F
	
	move $a0, $t2
	or $a1, $zero, 0x00040002
	li $v0, PRINT_INT
	syscall

	subu $sp, $sp, 12
	sw $ra,0($sp)
	sw $t1,4($sp)
	sw $t2,8($sp)

	li $a0, 500
	jal delay

	lw $t2,8($sp)
	lw $t1,4($sp)
	addiu $sp,$sp,12

	j main

	jr $ra

delay:	move $t0, $a0		 # void delay(int ms) {
d_for:	ble $t0, 0, d_ret	 # 	for(; ms>0; ms--) {
				 #
	subu $t0,$t0,1		 #
	li $v0, RESET_CORE_TIMER #
	syscall			 # 	resetCoreTimer();
d_wh:	li $v0, READ_CORE_TIMER	 #
	syscall			 #
	blt $v0,20000,d_wh	 # 	while(readCoreTimer() < 20000);
	j d_for			 # 	}
d_ret:	jr $ra			 # }

