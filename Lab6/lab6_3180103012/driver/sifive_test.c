#include "device.h"
#include "rinux_driver.h"

enum {
    SIFIVE_TEST_FAIL = 0x3333,
    SIFIVE_TEST_PASS = 0x5555,
};

static volatile unsigned int *test;

static void sifive_test_init() {
    test = (unsigned int *)(void *)PA2VA(get_device_addr(POWEROFF_MMIO));
}

static void sifive_test_poweroff() {
    *test = SIFIVE_TEST_PASS;
    while (1) {
        asm volatile("");
    }
}

poweroff_device_t poweroff_sifive_test = {sifive_test_init, sifive_test_poweroff};
