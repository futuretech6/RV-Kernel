#include <device.h>
#include <stdio.h>

void exit(int status) {
    poweroff_dev->poweroff();
    asm volatile("1: j 1b");
    __builtin_unreachable();
}