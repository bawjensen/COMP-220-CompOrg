	.data
ints:	.word	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
# ints:	.word 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
prompt:	.asciiz	"Enter up to 20 positive values, and enter a negative to end: "
oStr1:	.asciiz	"Minimum value: "
oStr2:	.asciiz	"Maximum value: "
oStr3:	.asciiz	"Integer mean: "
finStr:	.asciiz "Program finished. Goodbye."
endl:	.asciiz	"\n"

	.text
	.globl main
main:
	li	$t0, 0	# Index
	li	$t1, 4	# Incrementer
	li	$t2, 80	# End

	li	$s0, 0	# Current value
	li	$s1, 20	# Smallest value
	li	$s2, 0	# Largest value
	li	$s3, 0	# Sum

	# ------------------------------------------------------------------------

	li	$v0, 4
	la	$a0, prompt
	syscall			# Print first output label

	li	$v0, 4
	la	$a0, endl
	syscall			# Newline

loop1:
	beq	$t0, $t2, done_loop1 	# While loop boolean clause
	lw	$s0, ints($t0)

	slt	$t5, $s0, $s1 		# Check if s0 < t3, store in t5
	beq	$t5, $zero, lt_else 	# If less-than result is equal to zero, it was greater than, so goto else

	move	$s1, $s0

lt_else:
	slt	$t5, $s0, $s2
	bne	$t5, $zero, gt_else 	# If less-than result is not equal to zero, it was less than, so goto else

	move	$s2, $s0


gt_else:
	add	$s3, $s3, $s0
	add	$t0, $t0, $t1

	j	loop1

done_loop1:

	div	$t0, $t1	# Divide the final index by the memory size
	mflo	$t5		# Grab the quotient (# elements) and insert into t5

	div 	$s3, $t5	# Divide sum by num elements to get average
	mflo	$s3		# Overwrite sum with average

	# ------------------------------------------------------------------------

loop2:
	li	$t0, 0	# Index
	li	$t1, 4	# Incrementer


printing:

	li	$v0, 4
	la	$a0, oStr1
	syscall			# Print first output label

	li	$v0, 1
	move	$a0, $s1
	syscall			# Print smallest value

	li	$v0, 4
	la	$a0, endl
	syscall			# Newline

	# ------------------------------------------------------------------------

	li	$v0, 4
	la	$a0, oStr2
	syscall			# Print second output label

	li	$v0, 1
	move	$a0, $s2
	syscall			# Print largest value

	li	$v0, 4
	la	$a0, endl
	syscall			# Newline

	# ------------------------------------------------------------------------

	li	$v0, 4
	la	$a0, oStr3
	syscall			# Print third output label

	li	$v0, 1
	move	$a0, $s3
	syscall			# Print average

	li	$v0, 4
	la	$a0, endl
	syscall			# Newline

	# ------------------------------------------------------------------------

done:
	li	$v0, 4
	la	$a0, finStr
	syscall			# Print final goodbye

	li	$v0, 4
	la	$a0, endl
	syscall			# Newline

	# ------------------------------------------------------------------------

	li	$v0, 10		# 10 = system call code for halting program
	syscall			# call system function 10: exit