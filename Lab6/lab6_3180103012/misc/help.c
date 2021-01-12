#include <stdio.h>
#include <stdlib.h>

#include "buddy.h"
#include "slub.h"
#include "stdio.h"
#include "vm.h"

struct buddy buddy;

#define BIN_TREE_UPCHILD(__X) ((__X)*2 + 1)
#define BIN_TREE_DOWNCHILD(__X) ((__X)*2 + 2)
#define BIN_TREE_PARENT(__X) (((__X)-1) / 2)
#define ADDR_TO_INDEX(__A) ((__A))

/*
0 +---> 1 +---> 3 +---> 7
  |       |       |
  |       |       +---> 8
  |       |
  |       +---> 4 +---> 9
  |               |
  |               +---> 10
  |
  +---> 2 +---> 5 +---> 11
          |       |
          |       +---> 12
          |
          +---> 6 +---> 13
                  |
                  +---> 14
*/

void init_buddy_system(void) {
    buddy.pgnum = PAGE_FLOOR(BUDDY_SPACE_SIZE);

    int i = 0;
    for (uint64 layer_size = buddy.pgnum; layer_size; layer_size /= 2)
        for (uint64 node_count = 0; node_count < buddy.pgnum / layer_size; node_count++)
            buddy.bitmap[i++] = layer_size;

    alloc_pages(PAGE_FLOOR(KERNEL_PROG_SIZE));
}

void *alloc_pages(int npages) {

    int bm_loc         = 0;
    uint64 layer_size  = buddy.pgnum;
    uint64 ret_addr_pg = 0;  // Starting from physical 0x0

    while (1) {
        // layer_size check make sure BIN_TREE_CHILD will not overflow access
        if (layer_size > 1 && buddy.bitmap[BIN_TREE_UPCHILD(bm_loc)] >= npages)
            bm_loc = BIN_TREE_UPCHILD(bm_loc);
        else if (layer_size > 1 && buddy.bitmap[BIN_TREE_DOWNCHILD(bm_loc)] >= npages) {
            bm_loc = BIN_TREE_DOWNCHILD(bm_loc);
            ret_addr_pg += layer_size / 2;
        } else
            break;
        layer_size /= 2;
    }

    for (int i = bm_loc;; i = BIN_TREE_PARENT(i)) {  // sub alloc size from the parent tree
        buddy.bitmap[i] -= layer_size;
        if (!i)
            break;
    }

    return (void *)(BUDDY_START_ADDR + ret_addr_pg * PAGE_SIZE);
    // return (void *)PA2VA(BUDDY_START_ADDR + ret_addr_pg * PAGE_SIZE);
}

void free_pages(void *addr) {
    addr = VA2PA(addr);

    uint64 layer_size    = 1;
    _Bool size_not_found = 1;

    for (int bm_loc = buddy.pgnum - 1 + PAGE_FLOOR(addr - BUDDY_START_ADDR);;
         bm_loc     = BIN_TREE_PARENT(bm_loc)) {
        if (buddy.bitmap[bm_loc] && size_not_found) {
            layer_size <<= 1;
        } else {  // Found alloc page
            size_not_found = 0;
            buddy.bitmap[bm_loc] += layer_size;
            if (!bm_loc)  // Stop at root
                return;
        }
    }
}

int main(int argc, char const *argv[]) {
    init_buddy_system();
    printf("Init Done.\n");
    for (int i = 0; i < 2 * buddy.pgnum - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    void *tmpA = alloc_pages(1);
    for (int i = 0; i < 2 * buddy.pgnum - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    void *tmpB = alloc_pages(1);
    for (int i = 0; i < 2 * buddy.pgnum - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    void *tmpC = alloc_pages(4);
    for (int i = 0; i < 2 * buddy.pgnum - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    free_pages(tmpB);
    for (int i = 0; i < 2 * buddy.pgnum - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    return 0;
}
