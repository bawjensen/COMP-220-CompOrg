# hello.s
# "Hello, World!" program
# cparrish@sewanee.edu  6 Oct 2001

	.globl main
main:
	
	.data
str:	.asciiz "Test, World!"
	.text

	li $v0, 4	# system call code for print_str
	la $a0, str	# address of string to print
	syscall		# print the string