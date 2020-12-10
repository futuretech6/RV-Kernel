#include "put.h"

int puts(const char *s) {
    while (*s != '\0') {
        *UART_ADDR = (unsigned char)(*s);
        s++;
    }
    return 0;
}

static unsigned char hex_map[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

void putd(long x) {
    long digit = 1, tmp = x;
    while (tmp >= 10) {
        digit *= 10;
        tmp /= 10;
    }
    while (digit >= 1) {
        *UART_ADDR = hex_map[x / digit];
        x %= digit;
        digit /= 10;
    }
    return;
}

void putx(unsigned long x) {
    unsigned long digit = 1, tmp = x;

    puts("0x");
    if (x == 0) {
        puts("0");
        return;
    }
    while (tmp >= 16) {
        digit *= 16;
        tmp /= 16;
    }
    while (digit >= 1) {
        *UART_ADDR = hex_map[x / digit];
        x %= digit;
        digit /= 16;
    }
    return;
}
