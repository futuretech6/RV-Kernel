#ifndef _BUDDY_H
#define _BUDDY_H

#include "types.h"
#include "vm.h"

#define BUDDY_START_ADDR 0x80000000L
#define BUDDY_SPACE_SIZE (16 * 1024 * 1024)

struct buddy {
    unsigned long pgnum;                                           // number of `pages`
    unsigned long bitmap[2 * (BUDDY_SPACE_SIZE / PAGE_SIZE) - 1];  // bit map
};

void init_buddy_system(void);
void *alloc_pages(int npages);
void *alloc_pages_ret_pa(int npages);
void free_pages(void *addr);

#endif