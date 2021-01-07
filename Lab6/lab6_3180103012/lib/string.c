#include "string.h"

/**
 * @brief Memory set
 *
 * @param s Source address
 * @param c Char for replacement, of size 1 byte here
 * @param n Number of bytes replaced
 */
void *memset(void *s, int c, size_t n) {
    for (size_t i = 0; i < n; i++)
        *((uint8 *)s + i) = c;
    return s;
}

/**
 * @brief
 *
 * @param dest
 * @param src
 * @param n
 */
void memcpy(void *dest, const void *src, size_t n) {
    for (size_t i = 0; i < n; i++)
        *((uint8 *)dest + i) = *((uint8 *)src + i);
}