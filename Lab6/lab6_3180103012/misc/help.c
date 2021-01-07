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
    buddy.pgsize = 8;
    buddy.bitmap = (unsigned long *)malloc((2 * buddy.pgsize - 1) * sizeof(buddy.bitmap[0]));

    int i = 0;
    for (uint64 layer_size = buddy.pgsize; layer_size; layer_size /= 2)
        for (uint64 node_count = 0; node_count < buddy.pgsize / layer_size; node_count++)
            buddy.bitmap[i++] = layer_size;

    // alloc_pages(KERNEL_MAPPING_SIZE);
}
void *alloc_pages(int npages) {
    int bm_loc        = 0;
    uint64 alloc_size = buddy.pgsize;
    uint64 ret_val    = 0;  // Starting from physical 0x0

    while (1) {
        // alloc_size check make sure BIN_TREE_CHILD will not overflow access
        if (alloc_size > 1 && buddy.bitmap[BIN_TREE_UPCHILD(bm_loc)] >= npages)
            bm_loc = BIN_TREE_UPCHILD(bm_loc);
        else if (alloc_size > 1 && buddy.bitmap[BIN_TREE_DOWNCHILD(bm_loc)] >= npages) {
            bm_loc = BIN_TREE_DOWNCHILD(bm_loc);
            ret_val += alloc_size / 2;
        } else
            break;
        alloc_size /= 2;
    }

    for (int i = bm_loc;; i = BIN_TREE_PARENT(i)) {  // sub alloc size from the parent tree
        buddy.bitmap[i] -= alloc_size;
        if (!i)
            break;
    }

    return (void *)(ret_val * PAGE_SIZE);
}

void free_pages(void *addr) {
    addr                 = 0x2000;
    uint64 alloc_size    = 1;
    _Bool size_not_found = 1;

    for (int bm_loc = buddy.pgsize - 1 + PAGE_FLOOR(addr - BUDDY_START_ADDR);;
         bm_loc     = BIN_TREE_PARENT(bm_loc)) {
        if (buddy.bitmap[bm_loc] && size_not_found) {
            alloc_size <<= 1;
        } else {  // Found alloc page
            size_not_found = 0;
            buddy.bitmap[bm_loc] += alloc_size;
            if (!bm_loc)  // Stop at root
                return;
        }
    }
}

int main(int argc, char const *argv[]) {
    init_buddy_system();
    printf("Init Done.\n");
    for (int i = 0; i < 2 * buddy.pgsize - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    void *tmpA = alloc_pages(2);
    for (int i = 0; i < 2 * buddy.pgsize - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    void *tmpB = alloc_pages(1);
    for (int i = 0; i < 2 * buddy.pgsize - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    void *tmpC = alloc_pages(4);
    for (int i = 0; i < 2 * buddy.pgsize - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    free_pages(tmpA);
    for (int i = 0; i < 2 * buddy.pgsize - 1; i++)
        printf("%s%lu ", (i ? "" : "\n"), buddy.bitmap[i]);

    return 0;
}
