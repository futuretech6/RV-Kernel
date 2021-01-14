#ifndef _VM_H
#define _VM_H

#include "types.h"

#define KERNEL_PHY_BASE 0x80000000
#define KERNEL_VIR_BASE 0xffffffe000000000
#define USER_PHY_ENTRY 0x84000000
#define USER_STACK_TOP 0xffffffdf80000000

#define USER_MAPPING_SIZE 0x1000
#define KERNEL_MAPPING_SIZE 0x1000000  // 16MB

#ifndef PAGE_SIZE
#define PAGE_SIZE 0x1000  // 4096 bytes
#endif
#define PAGE_ENTRY_NUM 0x200  // 512

#define FREE_SPACE_SIZE 0x8000000  // [rt_pg_addr, rt_pg_addr + limit): 8MB

#define KERNEL_PROG_SIZE 0x1a000
#define KERNEL_TEXT_SIZE 0x5000
#define KERNEL_RODATA_SIZE 0x1000

#define PERM_R 0b10
#define PERM_W 0b100
#define PERM_X 0b1000
#define PROT_U 0b10000

#define UART_PHY_ADDR ((volatile unsigned char *)0x10000000)
#define UART_VIR_ADDR ((volatile unsigned char *)0xffffffdf90000000)

#define VA2PA(__VA) ((void *)((uint64)(__VA)-0xffffffdf80000000))
#define PA2VA(__PA) ((void *)((uint64)(__PA) + 0xffffffdf80000000))

#define PAGE_CEIL(__A) (((uint64)(__A)-1) / PAGE_SIZE + 1)
#define PAGE_FLOOR(__A) ((uint64)(__A) / PAGE_SIZE)

struct free_list_node {
    void *base;
    size_t limit;
    struct free_list_node *next;
};

/**
 * @brief Struct of PTE listed below:
 * PPN2 53:28; PPN1 27:19; PPN0 18:10; rsw 9:8; DAGUXWRV 8:0
 */
struct pageTable {
    uint64 PTE_list[512];
};

uint64 *page_walk(uint64 *pgtbl, uint64 va);
void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int prot);
void kernel_paging_init(void);
uint64 *user_paging_init(void);

#endif
