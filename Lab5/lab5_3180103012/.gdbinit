set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout asm

b trap_m
b trap_s
b thread_init
b start_kernel