#ifndef PUT_H
#define PUT_H

#define UART_ADDR (volatile unsigned char *)0x10000000
void putd(long num);
void putx(unsigned long num);
int puts(const char *s);

#endif
