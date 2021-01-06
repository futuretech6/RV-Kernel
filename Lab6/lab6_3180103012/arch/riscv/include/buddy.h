#ifndef _BUDDY_H
#define _BUDDY_H

#include "types.h"

struct buddy {
    unsigned long size;
    unsigned *bitmap;
};

void init_buddy_system(void);
void *alloc_pages(int);
void free_pages(void *);

#endif