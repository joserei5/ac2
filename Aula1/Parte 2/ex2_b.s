	.equ GET_CHAR, 2
	.equ PUT_CHAR, 3

	.data
	.text
	.globl main

main:	li $v0, GET_CHAR	# do{
	syscall			#
	move $t0, $v0		# c = getChar();
				#
	beq $v0,'\n',exit	# if ( c != '\n')
	addi $t0,$t0,1
	move $a0, $t0		#
	li $v0, PUT_CHAR	#
	syscall			#
	j main			# } while (c != '\n');

exit:	jr $ra
