        .data
A:      .ascii  "A"

        .text
        .globl main

main:

start:
        # TODO: EVERYTHING!

        j       start

        li      $v0, 10
        syscall