source ~/lab6/misc/gef.py
set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout reg

b trap_s
b kernel_paging_init
b set_page_attr
