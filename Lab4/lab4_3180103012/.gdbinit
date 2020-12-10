set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout asm

b *0x800000d8
b *0x800015dc
