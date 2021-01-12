set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout reg

b trap_s
b kernel_paging_init
b *0x800017b0
b *0x800000f8
b *0x800000bc
