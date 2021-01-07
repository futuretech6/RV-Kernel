#include "device.h"

console_device_t *console_dev   = &console_none;
poweroff_device_t *poweroff_dev = &poweroff_none;

void register_console(console_device_t *dev) {
    console_dev = dev;
    if (dev->init)
        dev->init();
}

void register_poweroff(poweroff_device_t *dev) {
    poweroff_dev = dev;
    if (dev->init)
        dev->init();
}

static char default_getchar() {
    asm volatile("ebreak");
    return 0;
}

static void default_putchar(char ch) {
    asm volatile("ebreak");
}

static void default_poweroff(int status) {
    asm volatile("ebreak");
    while (1) {
        asm volatile("" : : : "memory");
    }
}

console_device_t console_none = {NULL, default_getchar, default_putchar};

poweroff_device_t poweroff_none = {
    NULL,
    default_poweroff,
};

unsigned long get_device_addr(unsigned long key) {
    memmap_t *map = __mmio;
    if (key >= NULL_MMIO)
        return 0;
    else
        return __mmio[key].base;
}

unsigned long get_device_size(unsigned long key) {
    memmap_t *map = __mmio;
    if (key >= NULL_MMIO)
        return 0;
    else
        return __mmio[key].size;
}