        .data
loc:    .word   0
fArray: .float  1.1 2.2 3.3 4.4 5.5 6.6 7.7 8.8

val:    .asciiz "Value is: "
addr:   .asciiz "Address is: "
prompt: .asciiz "Enter loc: "
endl:   .asciiz "\n"

        .text
        .globl main

main:
        # Line #1
        li      $v0, 4
        la      $a0, val
        syscall

        li      $v0, 2
        l.s     $f12, fArray
        syscall

        li      $v0, 4
        la      $a0, endl
        syscall


        # Line #2
        li      $v0, 4
        la      $a0, addr
        syscall

        li      $v0, 1
        la      $a0, fArray
        syscall

        li      $v0, 4
        la      $a0, endl
        syscall


        # Line #3
        li      $v0, 4
        la      $a0, prompt
        syscall

        #Line #4
        li      $v0, 5
        syscall

        move    $t0, $v0
        sll     $t0, $t0, 2

        # Line #5
        li      $v0, 4
        la      $a0, val
        syscall

        li      $v0, 2
        l.s     $f12, fArray($t0)
        syscall

        li      $v0, 4
        la      $a0, endl
        syscall


        # Line #6
        li      $v0, 4
        la      $a0, addr
        syscall

        li      $v0, 1
        la      $a0, fArray($t0)
        syscall

        li      $v0, 4
        la      $a0, endl
        syscall


        

done:
        li      $v0, 10
        syscall