	.equ READ_INT10, 5
	.equ PRINT_INT, 6
	.equ PRINT_INT10,7
	.equ PRINT_STR, 8
	
	.data
msg1:	.asciiz "\nIntroduza um numero (sinal e modulo): "
msg2:	.asciiz "\nValor lido, em base 2: "
msg3:	.asciiz "\nValor lido, em base 16: "
msg4:	.asciiz "\nValor lido, em base 10 (unsigned): "
msg5:	.asciiz "\nValor lido, em base 10 (signed): "

	.text
	.globl main

main:	la $a0, msg1
	li $v0,PRINT_STR
	syscall
	
	li $v0, READ_INT10
	syscall
	move $t0, $v0

	la $a0, msg2
	li $v0, PRINT_STR
	syscall

	move $a0,$t0
	li $a1, 2
	li $v0, PRINT_INT
	syscall	

	la $a0, msg3
	li $v0, PRINT_STR
	syscall

	move $a0, $t0
	li $a1, 16
	li $v0, PRINT_INT
	syscall

	la $a0, msg4
	li $v0, PRINT_STR
	syscall

	move $a0, $t0
	li $a1, 10
	li $v0, PRINT_INT
	syscall
	
	la $a0, msg5
	li $v0, PRINT_STR
	syscall

	move $a0, $t0
	li $v0, PRINT_INT10
	syscall

exit:	jr $ra
