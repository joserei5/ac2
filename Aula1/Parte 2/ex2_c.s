	.equ INKEY, 1
	.equ PUT_CHAR, 3
	.data
	.text
	.globl main

main:	li $t0, 0	# char c;
	
while1:	beq $t0, '\n', exit
	li 
