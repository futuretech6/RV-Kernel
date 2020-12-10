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

void *kalloc_byte(size_t size) {
    static struct free_list_node *free_list = NULL;
    if (free_list == NULL) {  // Init
        asm("la t0, _end");
        asm("sd t0, %0" ::"m"(free_list));
        free_list->base  = (void *)0x80000000;
        free_list->limit = 0x1000000;  // 16MB
        free_list->next  = NULL;
    }
    for (struct free_list_node *p = free_list; p; p = free_list->next) {
        if (p->limit >= size) {
            p->limit -= size;
            return (void *)((uint8 *)p->base + p->limit - size);
        }
    }
}

void memset_byte(void *s, uint8 c, size_t n) {
    for (int i = 0; i < n; i++)
        *((uint8 *)s + i) = c;
}

uint64 *page_walk(uint64 *pgtbl, uint64 va, _Bool alloc) {
    for (int level = 2; level > 0; level--, pgtbl = (uint64 *)((uint64)pgtbl + PAGE_SIZE)) {
        uint64 *pte;
        switch (level) {
            case 2: pte = &pgtbl[VAtoVPN2(va)]; break;
            case 1: pte = &pgtbl[VAtoVPN1(va)]; break;
        }
        // Don' check
        // if (PTEtoV(*pte))
        //     pgtbl = (uint64 *)PTEtoPA(*pte);
        // else {  // Invalid
        //     if (!alloc || (pgtbl == (uint64 *)kalloc_byte(PAGE_SIZE)) == NULL)
        //         return NULL;
        // memset_byte(pgtbl, (uint8)0, PAGE_SIZE);
        LoadPTE(*pte, PAtoPPN((uint64)pgtbl + PAGE_SIZE), 0, 1);
        // }
    }
    return &pgtbl[VAtoVPN0(va)];
}

/**
 * @brief Create a mapping object
 *
 * @param pgtbl 根页表的基地址
 * @param va 需要映射的虚拟地址的基地址
 * @param pa 需要映射的物理地址的基地址
 * @param sz 映射的大小
 * @param perm 映射的读写权限
 */
int create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {
    uint64 *pte;  // PTE of level0

    for (uint64 v_addr = Page_Floor(va); v_addr != Page_Floor(va + sz - 1);
         v_addr += PAGE_SIZE, pa += PAGE_SIZE, pgtbl += PAGE_SIZE) {
        if ((pte = page_walk(pgtbl, v_addr, 1)) == NULL)  // Alloc error
            return -1;
        if (PTEtoV(*pte)) {  // Invalid, page fault
            // remap error?
        }
        LoadPTE(*pte, PAtoPPN(pa), perm, 1);
    }
    return 0;

    // if (pgtbl)
    //     pRootPT = (struct pageTable *)pgtbl;

    // PPN2toPTE(pRootPT->PTE_list[VAtoVPN2(va)], PAtoPPN2(pa));
    // PROTtoPTE(pRootPT->PTE_list[VAtoVPN2(va)], 0, 1);
    // PPN1toPTE(pRootPT->PTE_list[VAtoVPN1(va)], PAtoPPN1(pa));
    // PROTtoPTE(pRootPT->PTE_list[VAtoVPN1(va)], 0, 1);
    // PPN0toPTE(pRootPT->PTE_list[VAtoVPN0(va)], PAtoPPN1(pa));
    // PROTtoPTE(pRootPT->PTE_list[VAtoVPN0(va)], perm, 1);  // Only the last one has RWX
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
    putx((uint64)_end_addr);
    create_mapping(  // 等值映射
        _end_addr, 0x80000000, 0x80000000, 0x1000000, PERM_R | PERM_W | PERM_X);
    create_mapping(  // 高位映射
        _end_addr, 0xffffffe000000000, 0x80000000, 0x1000000, PERM_R | PERM_W | PERM_X);
    create_mapping(  // 映射UART
        _end_addr, 0x10000000, 0x10000000, 0x1000000, PERM_R | PERM_W | PERM_X);
}