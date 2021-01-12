#ifndef _BUDDY_H
#define _BUDDY_H

#include "types.h"
#include "vm.h"

#define BUDDY_START_ADDR 0x80000000L
#define BUDDY_SPACE_SIZE (16 * 1024 * 1024)

#define VA2PA(__VA) ((__VA)-0xffffffdf80000000)
#define PA2VA(__PA) ((__PA) + 0xffffffdf80000000)

struct buddy {
    unsigned long pgnum;                                           // number of `pages`
    unsigned long bitmap[2 * (BUDDY_SPACE_SIZE / PAGE_SIZE) - 1];  // bit map
};

void init_buddy_system(void);
void *alloc_pages(int, _Bool retVA = 1);
void free_pages(void *);

#endif