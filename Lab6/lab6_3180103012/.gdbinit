source ~/lab6/misc/gef.py
set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout src

b do_page_fault