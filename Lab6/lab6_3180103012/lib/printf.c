#include "device.h"
#include "stdio.h"
#include "stdlib.h"

int putchar(int ch) {
    // console_dev->putchar(ch);
    *((char *)0xffffffdf90000000) = ch;
    return 1;
}

int puts(const char *s) {
    while (*s)
        putchar(*s++);
}

static int vprintfmt(int (*putch)(int), const char *fmt, va_list vl) {
    int in_format = 0, longarg = 0;
    size_t pos = 0;
    for (; *fmt; fmt++) {
        if (in_format) {
            switch (*fmt) {
                case 'l': {
                    longarg = 1;
                    break;
                }

                case 'x': {
                    long num = longarg ? va_arg(vl, long) : va_arg(vl, int);

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

                    for (int i = bits; i >= 0; i--) {
                        putch(decchar[i]);
                    }
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

                    for (int i = bits; i >= 0; i--) {
                        putch(decchar[i]);
                    }
                    pos += bits + 1;
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
    return pos;
}

int printf(const char *s, ...) {
    int res = 0;
    va_list vl;
    va_start(vl, s);
    res = vprintfmt(putchar, s, vl);
    va_end(vl);
    return res;
}

void panic(const char *s, ...) {
    printf("panic: ");

    va_list vl;
    va_start(vl, s);
    vprintfmt(putchar, s, vl);
    va_end(vl);

    printf("\n");
    exit(0);
}