# Joe Schmoe
# Purpose: to get annoyed
# Input:   none
# Output:  not much

#----------------------------------------------------------------------------#
# Start with the "data" section, which one can think of as global constants
	.data
str:	.asciiz	"They killed Kenny Derp!"

#----------------------------------------------------------------------------#
# Next is the "text" section, which is the executable code

	.text

# The main starts here
	.globl main

main:
	li	$v0, 4  	# li = load immediate
				# 4 = system call code for printing a string
				# v0 = register for system call codes
	la	$a0, str        # la = load address
				# load str defined above into register $a0
				# available registers are $a0-$a3, 
				# and $f12 for floats
	syscall			# call the system function 4: print

# Last step is to halt the program
	li	$v0, 10		# 10 = system call code for halting program
	syscall			# call system function 10: exit
