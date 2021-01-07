#ifndef _BUDDY_H
#define _BUDDY_H

#include "types.h"

#define BUDDY_START_ADDR 0x0L
#define BUDDY_SPACE_SIZE 0x80000000L

struct buddy {
    unsigned long pgsize;   // number of `pages`
    unsigned long *bitmap;  // bit map
};

void init_buddy_system(void);
void *alloc_pages(int);
void free_pages(void *);

#endif