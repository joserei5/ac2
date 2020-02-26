	.equ READ_CORE_TIMER, 11
	.equ RESET_CORE_TIMER, 12
	.equ PUT_CHAR, 3

	.data
	.text
	.globl main

main:	jr $ra

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

