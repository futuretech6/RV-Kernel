set breakpoint pending on
set output-radix 10
target remote localhost:1234
layout src
b do_timer
b switch_to
b context_save
b context_load
