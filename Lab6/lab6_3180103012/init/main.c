#include "device.h"
#include "sched.h"
#include "stdio.h"

#define SIFIVE_TEST 0x100000
#define VIRT_TEST_FINISHER_PASS 0x5555

memmap_t __mmio[] = {[UART_MMIO] = {0x10000000, 0x100},
    [CLINT_MMIO]                 = {0x2000000, 0x10000},
    [PLIC_MMIO]                  = {0xc000000, 0x4000000},
    [POWEROFF_MMIO]              = {0x100000, 0x1000},
    [DRAM_MMIO]                  = {0x80000000, 0x0},
    [NULL_MMIO]                  = {0x0, 0x0}};

void device_init() {
    register_console(&console_ns16550a);
    register_poweroff(&poweroff_sifive_test);
}

int os_test() {
    puts("ZJU OS LAB 5             3180103012/GROUP-17\n");
    puts("task init...\n");

    task_init();
    for (;;)
        ;

    return 0;
}

int start_kernel() {
    os_test();
    asm("sh %0, 0(%1)" : : "r"(VIRT_TEST_FINISHER_PASS), "r"(SIFIVE_TEST));
    return 0;
}
