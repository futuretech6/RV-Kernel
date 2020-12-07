set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout asm
b schedule
b do_timer
