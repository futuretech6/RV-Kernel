/**
 * @file vm.c
 * @author Scott Chen
 * @brief the implementation vm management of oslab4
 * @version 0.1
 * @date 2020-12-05
 * @ref https://gitee.com/zjuicsr/lab20fall-stu/wikis/lab4
 * @ref http://www.five-embeddev.com/riscv-isa-manual/latest/supervisor.html#sv32algorithm
 */
#include "vm.h"
#include "put.h"

/**
 * @brief Alloc physical memory space for kernel
 *
 * @param size Memory space, in unit byte
 * @return void* Allocated space, NULL for insufficient space
 */
void *kalloc_byte(size_t size) {
    static struct free_list_node first_node;
    static struct free_list_node *free_list = NULL;
    if (free_list == NULL) {  // Init
        free_list = &first_node;
        asm("la t0, _end");
        asm("sd t0, %0" ::"m"(free_list->base));
        free_list->limit = 0x10000000;  // 256MB // 16MB
        free_list->next  = NULL;
    }
    for (struct free_list_node *p = free_list; p; p = free_list->next) {
        if (p->limit >= size) {
            p->limit -= size;
            return (void *)((uint8 *)p->base + p->limit - size);
        } else
            return NULL;
    }
}

/**
 * @brief Memory set
 *
 * @param s Source address
 * @param c Char for replacement, of size 1 byte here
 * @param n Number of bytes replaced
 */
void memset_byte(void *s, uint8 c, size_t n) {
    for (int i = 0; i < n; i++)
        *((uint8 *)s + i) = c;
}

/**
 * @brief Page walk from level-2 PT to level-1 PT, and return addr of level-0 PTE
 *
 * @param pgtbl Addr of 1st(level-2) page table
 * @param va VA
 * @return uint64* Address of pte of the 3rd(level-0) PTE
 */
uint64 *page_walk(uint64 *pgtbl, uint64 va) {
    for (int level = 2; level > 0; level--) {
        uint64 *pte_addr;
        switch (level) {
            case 2: pte_addr = &pgtbl[VAtoVPN2(va)]; break;
            case 1: pte_addr = &pgtbl[VAtoVPN1(va)]; break;
        }
#if PAGING_DEBUG
        puts("\n==== in `page_walk`");
        puts("\nlevel: ");
        putd(level);
        puts("\npgtbl: ");
        putx(pgtbl);
        puts("\nva: ");
        putx(va);
        puts("\npte_addr: ");
        putx(pte_addr);
        puts("\n*pte_addr: ");
        putx(*pte_addr);
        puts("\n");
#endif
        // Update next level's pgtbl
        if (PTEtoV(*pte_addr)) {  // Valid PTE, next level PT has been constructed
            pgtbl = (uint64 *)(PTEtoPPN(*pte_addr) << 12);
        } else {  // Invalid PTE, no next level PT
            pgtbl = kalloc_byte(PAGE_SIZE);
            memset_byte(pgtbl, 0, PAGE_SIZE);
            LoadPTE(*pte_addr, PAtoPPN((uint64)pgtbl), 0, 1);
        }
    }
    return &pgtbl[VAtoVPN0(va)];
}

/**
 * @brief Create a mapping object
 *
 * @param pgtbl Base addr of level-2 page table
 * @param va Mapping from
 * @param pa Mapping to
 * @param sz Mapping size
 * @param perm Mapping permission: XWR
 */
void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {
#if PAGING_DEBUG
    static int cnt = 0;
    puts("\n==== In `create_mapping`");
    puts("\ncnt: ");
    putd(cnt++);
    puts("\n");
#endif
    for (uint64 addr_last_byte = va + sz - 1; va <= addr_last_byte;
         va += PAGE_SIZE, pa += PAGE_SIZE) {
#if PAGING_DEBUG
        puts("\n==== In `create_mapping`");
        puts("\npgtbl: ");
        putx(pgtbl);
        puts("\nva: ");
        putx(va);
        puts("\nVPN2: ");
        putx(VAtoVPN2(va));
        puts(", VPN1: ");
        putx(VAtoVPN1(va));
        puts(", VPN0: ");
        putx(VAtoVPN0(va));
        puts("\npa: ");
        putx(pa);
#endif
        uint64 *pte_addr = page_walk(pgtbl, va);  // PTE of level0
        LoadPTE(*pte_addr, PAtoPPN(pa), perm, 1);
#if PAGING_DEBUG
        puts("\n==== In `create_mapping`");
        puts("\npte_addr: ");
        putx(pte_addr);
        puts("\npte: ");
        putx(*pte_addr);
        puts("\n");
#endif
    }
}

/**
 * @brief 将内核起始的0x80000000的16MB映射到0xffffffe000000000，同时也进行等值映射。
 * 将必要的硬件地址（如UART）进行等值映射，无偏移
 *
 */
void paging_init(void) {
    uint64 *_end_addr;
    asm("la t0, _end");
    asm("sd t0, %0" ::"m"(_end_addr));
    create_mapping(  // 等值映射
        _end_addr, MAPPING_BASE_P, MAPPING_BASE_P, MAPPING_LIMIT, PERM_R | PERM_W | PERM_X);
    create_mapping(  // 高位映射
        _end_addr, MAPPING_BASE_V, MAPPING_BASE_P, MAPPING_LIMIT, PERM_R | PERM_W | PERM_X);
    create_mapping(  // 映射UART
        _end_addr, (uint64)UART_ADDR, (uint64)UART_ADDR, 0x1, PERM_R | PERM_W | PERM_X);
}