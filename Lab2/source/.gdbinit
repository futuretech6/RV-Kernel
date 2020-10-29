set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout reg
b _start
b trap_s
b trap_m_timer
b trap_m_ecallS
display (long)*0x200bff8
display (long)*0x2004000
