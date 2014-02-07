# Joe Schmoe
# Purpose: to get annoyed
# Input:   none
# Output:  not much

#----------------------------------------------------------------------------#
# Start with the "data" section, which one can think of as global constants
	.data
intArray:	.word 0 1 2 3 4 5 6 7 8 9
# instr:	.asciiz	"Enter three integers: "
# outstr:	.asciiz	"The sum is: "
# divis:	.asciiz	"The Division: "
# remain:	.asciiz	"The remainder is: "

# endl:	.asciiz	"\n"


#----------------------------------------------------------------------------#
# Next is the "text" section, which is the executable code

	.text

# The main starts here
	.globl main

main:
	# Setting up register variables
	li	$t0, 0	# sum
	li	$t2, 0	# current offset

	# Loading first word
	lw	$t1, intArray($t2) # Load from current offset into t1
	add	$t0, $t0, $t1 # += into sum

	# Loading second word
	addi	$t2, 4 # shift offset by 4 bytes
	lw	$t1, intArray($t2) # Load from current offset into t1
	add	$t0, $t0, $t1	# += into sum

	# Loading third word
	addi	$t2, 4 # shift offset by 4 bytes
	lw	$t1, intArray($t2) # Load from current offset into t1
	add	$t0, $t0, $t1	# += into sum

	# Print out sum of three
	li	$v0, 1 			# load "print_int"
	move	$a0, $t0 	# Move to arg position
	syscall 			# print out


	# li	$v0, 4
	# la	$a0, instr
	# syscall

	# li	$v0, 5
	# syscall

	# add	$t0, $v0, 0

	# li	$v0, 5
	# syscall

	# add	$t1, $v0, 0

	# li	$v0, 5
	# syscall

	# add	$t2, $v0, 0

	# li	$v0, 4
	# la 	$a0, outstr
	# syscall

	# add	$t3, $t1, $t2

	# add	$t3, $t3, $t0

	# add	$a0, $t3, 0

	# li	$v0, 1 
	# syscall

	# li	$v0, 4
	# la	$a0, endl
	# syscall

	# li	$v0, 4
	# la	$a0, divis
	# syscall


	# div	$t4, $t3, 3		# Goes into lo(quotient) and hi(remainder)

	# add	$a0, $t4, 0

	# li 	$v0, 1
	# syscall

	# li	$v0, 4
	# la	$a0, endl
	# syscall




	# add	$a0, $t3, 0
	# li	$v0, 1
	# syscall

# Last step is to halt the program
	li	$v0, 10		# 10 = system call code for halting program
	syscall			# call system function 10: exit
