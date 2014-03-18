	.data
intArray:	.word	11 59 2 97 -4 -66 -45 22 19 -34
quarter:	.asciiz "quarter"
dime:		.asciiz "dime"
nickel:		.asciiz "nickel"
penny:		.asciiz "penny"


endl:	.asciiz	"\n"

	.text

	.globl main

main:
	

done:
	li	$v0, 10		# 10 = system call code for halting program
	syscall			# call system function 10: exit