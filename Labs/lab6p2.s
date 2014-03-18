        .data
one:    .float  0.0
two:    .float  0.0
three:  .float  0.0

prompt: .asciiz "Enter a float: "
endl:   .asciiz "\n"

        .text
        .globl main
main:
        li      $v0, 4
        la      $a0, prompt
        syscall

        li      $v0, 6
        syscall

        mov.s   $f2, $f0

        li      $v0, 4
        la      $a0, endl
        syscall



        li      $v0, 4
        la      $a0, prompt
        syscall

        li      $v0, 6
        syscall

        mov.s   $f1, $f0

        li      $v0, 4
        la      $a0, endl
        syscall



        li      $v0, 4
        la      $a0, prompt
        syscall

        li      $v0, 6
        syscall

        li      $v0, 4
        la      $a0, endl
        syscall



        add.s   $f4, $f0, $f1
        add.s   $f4, $f4, $f2

        li      $t0, 3
        mtc1    $t0, $f6
        cvt.s.w $f6, $f6
        div.s   $f4, $f4, $f6

        li      $v0, 2
        mov.s   $f12, $f4
        syscall

        li      $v0, 4
        la      $a0, endl
        syscall

        cvt.d.s $f4, $f6
        cvt.d.s $f6, $f0
        cvt.d.s $f8, $f1
        cvt.d.s $f10, $f2

        add.d   $f0, $f6, $f8
        add.d   $f0, $f0, $f10

        div.d   $f0, $f0, $f4
        

        li      $v0, 3
        mov.d   $f12, $f0
        syscall

        li      $v0, 4
        la      $a0, endl
        syscall 



        li      $v0, 10
        syscall