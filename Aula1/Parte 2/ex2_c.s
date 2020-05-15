	.equ INKEY, 1
	.equ PUT_CHAR, 3
	.data
	.text
	.globl main

main:	li $t0, 0		# char c;
	
while1:	beq $t0, '\n', exit	# while( c != '\n'){
while2:	li $v0, INKEY		# 
	syscall			#
	move $t0, $v0		# c = inkey();
	beq $t0, 0, while2	# while( (c = inkey()) == 0 );
if:	beq $t0, '\n', endif	# if( c!= '\n')
	move $a0, $t0		#    putChar(c);
	li $v0,PUT_CHAR		#
	syscall			#
endif:	j while1		# }

exit:	jr $ra			# return 0;
