set breakpoint pending on
set output-radix 16
target remote localhost:1234
layout reg
b _start
b trap_s
b trap_m_timer
b trap_m_ecallS
