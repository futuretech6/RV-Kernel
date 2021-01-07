#pragma once

#include "stddef.h"

// TODO: support other boards
enum { UART_MMIO, CLINT_MMIO, PLIC_MMIO, POWEROFF_MMIO, DRAM_MMIO, NULL_MMIO };

typedef struct memmap {
    unsigned long base;
    unsigned long size;
} memmap_t;

extern memmap_t __mmio[];

unsigned long get_device_addr(unsigned long key);
unsigned long get_device_size(unsigned long key);

// UART
typedef struct console_device {
    void (*init)();
    char (*getchar)();
    void (*putchar)(char);
} console_device_t;

void register_console(console_device_t *dev);
extern console_device_t *console_dev;
extern console_device_t console_none;
extern console_device_t console_ns16550a;

// POWEROFF
typedef struct poweroff_device {
    void (*init)();
    void (*poweroff)();
} poweroff_device_t;

void register_poweroff(poweroff_device_t *dev);
extern poweroff_device_t *poweroff_dev;
extern poweroff_device_t poweroff_none;
extern poweroff_device_t poweroff_sifive_test;