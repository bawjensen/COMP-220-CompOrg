	.data
intArray:	.word 11 59 2 97 -4 -66 -45 22 19 -34

endl:	.asciiz	"\n"

	.text

	.globl main

main:
	li	$t0, 0 # Start
	li	$t1, 40 # End
	li	$t2, 4 # Step

loop:
	beq	$t0, $t1, after  # while

	lw	$t3, intArray($t0) # Load next int
	slt	$t4, $t3, $zero # < check

	beq	$t4, $zero, else # If greater than, skip over print

	li	$v0, 1
	lw 	$a0, intArray($t0)
	syscall

	li	$v0, 4
	la	$a0, endl
	syscall

else:
	add	$t0, $t0, $t2

	j	loop            # loop back up

after:
	li	$t0, 0

loop2:
	beq	$t0, $t1, done  # while

	lw	$t3, intArray($t0) # Load next int
	slt	$t4, $t3, $zero # < check

	bne	$t4, $zero, else2 # If greater than, skip over print

	li	$v0, 1
	lw 	$a0, intArray($t0)
	syscall

	li	$v0, 4
	la	$a0, endl
	syscall

else2:
	add	$t0, $t0, $t2

	j	loop2            # loop back up

done:
	li	$v0, 10		# 10 = system call code for halting program
	syscall			# call system function 10: exit
