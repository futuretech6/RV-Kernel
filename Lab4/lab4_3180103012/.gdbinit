set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout asm


b *0xffffffe000001004
b *0xffffffe000000ff4
