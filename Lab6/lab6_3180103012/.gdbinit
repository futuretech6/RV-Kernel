source ~/lab6/misc/gef.py
set breakpoint pending on
set output-radix 16
target remote localhost:1234
layout src

b do_page_fault
b thread_init
b trap_s
