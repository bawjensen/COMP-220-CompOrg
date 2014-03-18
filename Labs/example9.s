#----------------------------------------------------------------------------#
# example9.a
#
# Joe Schmoe
# Purpose: floating point arithmetic
# Input:   radius of a circle
# Output:  circumference of circle, using floats and doubles (2piR)
#----------------------------------------------------------------------------#
	.data
hello:  .asciiz "\nFloating point example.\n"
prompt: .asciiz "Enter radius: "
endl:	.asciiz "\n"
float:	.asciiz "Radius (float answer) : "
double:	.asciiz "Radius (double answer): "
pi:	.float 3.1415926
bigpi:	.double 3.1415926		# note that double == double word
#----------------------------------------------------------------------------#
	.text
	.globl main
main:
	li      $v0, 4
        la      $a0, hello
        syscall

# read in radius
        la      $a0, prompt
        syscall
	li	$v0, 6			# 6 = code for reading float
	syscall				# input value in $f0

	l.s 	$f1, pi			# $f1 = pi
	add.s	$f1, $f1, $f1		# 2 pi
	mul.s	$f2, $f1, $f0		# $f2 = 2piR

# move result to output register
	li	$v0, 4
	la	$a0, float
	syscall

	mov.s	$f12, $f2
	li	$v0, 2			# code for printing single = 2
	syscall
# print out endl 
	li      $v0, 4
        la      $a0, endl
	syscall

# Try the same thing with doubles
	li	$v0, 4
	la	$a0, double
	syscall

# To store doubles, use a register pair, starting with an even register.
# The value is accessed by the name of the even register.
# Make sure you don't overwrite the second register!

# First, convert the single in $f0 (radius) to a double.
	cvt.d.s	$f0, $f0
	l.d	$f2, bigpi
	add.d	$f4, $f2, $f2		# 2 pi
	mul.d	$f4, $f4, $f0		# 2piR
 	mov.d	$f12, $f4
	li	$v0, 3			# code for printing double = 3
	syscall

# print out endl (twice)
	li      $v0, 4
        la      $a0, endl
        syscall
        la      $a0, endl
        syscall

done:
	li	$v0, 10		
	syscall			
