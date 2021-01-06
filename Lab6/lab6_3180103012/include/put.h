#ifndef PUT_H
#define PUT_H

#define UART_PHY_ADDR ((volatile unsigned char *)0x10000000)
#define UART_VIR_ADDR ((volatile unsigned char *)0xffffffdf90000000)

void putd(long num);
void putx(unsigned long num);
int puts(const char *s);
void putf(const char *s, ...);

#endif
