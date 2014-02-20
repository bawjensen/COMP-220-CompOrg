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
	li	$t0, 0 # Start
	li	$t1, 40 # End
	li	$t2, 4 # Step

	li	$s0, 25
	li	$s1, 10
	li	$s2, 5
	li	$s3, 1

	li	$v0, 4
	la	$a0, quarter
	syscall
loop:
	lw	$t3, intArray($t0)
	slt	$t4, $t3, $zero
	beq	$t4, $zero, done # If comparison == 0, goto done

	beq	$t0, $t1, done

	divu	$t3, $s0
	mflo	$t6

	sgt	$t5, $t6, $zero
	beq	$t5, $zero, ten

	li	$v0, 4
	la	$a0, quarter
	syscall

	li	$v0, 1
	move	$a0, $t0
	syscall


ten:
	add	$t0, $t0, $t2

	j	loop

done:
	li	$v0, 10		# 10 = system call code for halting program
	syscall			# call system function 10: exit