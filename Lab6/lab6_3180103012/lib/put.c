#include "put.h"
#include "types.h"

typedef __builtin_va_list va_list;
#define va_start(v, l) __builtin_va_start(v, l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v, l) __builtin_va_arg(v, l)
#define va_copy(d, s) __builtin_va_copy(d, s)

int puts(const char *s) {
    while (*s != '\0') {
        *UART_VIR_ADDR = (unsigned char)(*s);
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
        *UART_VIR_ADDR = hex_map[x / digit];
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
        *UART_VIR_ADDR = hex_map[x / digit];
        x %= digit;
        digit /= 16;
    }
    return;
}

int tail                 = 0;
static char buffer[1000] = {[0 ... 999] = 0};

static inline int putchar(int c) {
    buffer[tail++] = (char)c;
    return 0;
}

static void vprintfmt(int (*putch)(int), const char *fmt, va_list vl) {
    _Bool in_format = 0, longarg = 0;
    size_t pos = 0;
    for (; *fmt; fmt++) {
        if (in_format) {
            switch (*fmt) {
                case 'l': {
                    longarg = 1;
                    break;
                }
                case 'x': {
                    long num      = longarg ? va_arg(vl, long) : va_arg(vl, int);
                    int hexdigits = 2 * (longarg ? sizeof(long) : sizeof(int)) - 1;
                    for (int halfbyte = hexdigits; halfbyte >= 0; halfbyte--) {
                        int hex      = (num >> (4 * halfbyte)) & 0xF;
                        char hexchar = (hex < 10 ? '0' + hex : 'a' + hex - 10);
                        putch(hexchar);
                        pos++;
                    }
                    longarg   = 0;
                    in_format = 0;
                    break;
                }
                case 'd': {
                    long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                    if (num < 0) {
                        num = -num;
                        putch('-');
                        pos++;
                    }
                    int bits         = 0;
                    char decchar[25] = {'0', 0};
                    for (long tmp = num; tmp; bits++) {
                        decchar[bits] = (tmp % 10) + '0';
                        tmp /= 10;
                    }
                    if (bits == 0)
                        bits++;
                    for (int i = bits - 1; i >= 0; i--)
                        putch(decchar[i]);
                    pos += bits + 1;
                    longarg   = 0;
                    in_format = 0;
                    break;
                }

                case 'u': {
                    unsigned long num = longarg ? va_arg(vl, long) : va_arg(vl, int);
                    int bits          = 0;
                    char decchar[25]  = {'0', 0};
                    for (long tmp = num; tmp; bits++) {
                        decchar[bits] = (tmp % 10) + '0';
                        tmp /= 10;
                    }
                    if (bits == 0)
                        bits++;
                    for (int i = bits - 1; i >= 0; i--)
                        putch(decchar[i]);
                    pos += bits - 1;
                    longarg   = 0;
                    in_format = 0;
                    break;
                }

                case 's': {
                    const char *str = va_arg(vl, const char *);
                    while (*str) {
                        putch(*str);
                        pos++;
                        str++;
                    }
                    longarg   = 0;
                    in_format = 0;
                    break;
                }

                case 'c': {
                    char ch = (char)va_arg(vl, int);
                    putch(ch);
                    pos++;
                    longarg   = 0;
                    in_format = 0;
                    break;
                }
                default: break;
            }
        } else if (*fmt == '%') {
            in_format = 1;
        } else {
            putch(*fmt);
            pos++;
        }
    }

    long syscall_ret, fd = 1;
    buffer[tail++] = '\0';
    puts(buffer);
}

void putf(const char *s, ...) {
    va_list vl;
    va_start(vl, s);
    tail = 0;
    vprintfmt(putchar, s, vl);
    va_end(vl);
}