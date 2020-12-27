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
void *kalloc(size_t size) {
    static struct free_list_node first_node;
    static struct free_list_node *free_list = NULL;
    if (free_list == NULL) {  // Init
        free_list = &first_node;
        asm("la t0, rt_pg_addr");
        asm("sd t0, %0" ::"m"(free_list->base));
        free_list->limit = FREE_SPACE_SIZE;
        free_list->next  = NULL;
    }
    for (struct free_list_node *p = free_list; p; p = free_list->next)
        if (p->limit >= size) {
            p->limit -= size;
            return (void *)((uint8 *)(p->base = (uint8 *)p->base + size) - size);
        } else
            return NULL;
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
        // Update `pgtbl` to be next level's pg tbl's base
        if (PTEtoV(*pte_addr)) {  // Valid PTE, next level PT has been constructed
            pgtbl = (uint64 *)(PTEtoPPN(*pte_addr) << 12);
        } else {  // Invalid PTE, need to construct next level PT
            if ((pgtbl = (uint64 *)kalloc(PAGE_SIZE)) == NULL) {
                puts("\n[!] Insufficient Free Space.\n");
                return NULL;  // Insufficient free space for pg tbls
            }
            memset_byte(pgtbl, 0, PAGE_SIZE);
            LoadPTE(pte_addr, PAtoPPN((uint64)pgtbl), 0, 1);  // PTE <- next level pg's PPN
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
 * @param sz Mapping size, ceil to PAGE_SIZE
 * @param prot Mapping protetion & permission(XWR)
 */
void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int prot) {
    for (uint64 addr_last_byte = va + sz - 1; va <= addr_last_byte;
         va += PAGE_SIZE, pa += PAGE_SIZE) {
        LoadPTE(page_walk(pgtbl, va), PAtoPPN(pa), prot, 1);
    }
}

/**
 * @brief 将内核起始的0x80000000的16MB映射到0xffffffe000000000，同时也进行等值映射。
 * 将必要的硬件地址（如UART）进行等值映射，无偏移。
 */
void paging_init(void) {
    uint64 *rtpg_addr = (uint64 *)kalloc(PAGE_SIZE);
    // Map UART
    create_mapping(
        rtpg_addr, (uint64)UART_VIR_ADDR, (uint64)UART_PHY_ADDR, PAGE_SIZE, PERM_R | PERM_W);

    // Map Kernel: High
    create_mapping(rtpg_addr, KERNEL_VIR_BASE, KERNEL_PHY_BASE, KERNEL_TEXT_SIZE,
        PERM_R | PERM_X);  // text
    create_mapping(rtpg_addr, KERNEL_VIR_BASE + KERNEL_TEXT_SIZE,
        KERNEL_PHY_BASE + KERNEL_TEXT_SIZE, KERNEL_RODATA_SIZE,
        PERM_R);  // rodata
    create_mapping(rtpg_addr, KERNEL_VIR_BASE + KERNEL_TEXT_SIZE + KERNEL_RODATA_SIZE,
        KERNEL_PHY_BASE + KERNEL_TEXT_SIZE + KERNEL_RODATA_SIZE,
        KERNEL_MAPPING_SIZE - (KERNEL_TEXT_SIZE + KERNEL_RODATA_SIZE),
        PERM_R | PERM_W);  // Other Sections

    // Map Kernel: Equal
    create_mapping(rtpg_addr, KERNEL_PHY_BASE, KERNEL_PHY_BASE, KERNEL_TEXT_SIZE,
        PERM_R | PERM_X);  // text
    create_mapping(rtpg_addr, KERNEL_PHY_BASE + KERNEL_TEXT_SIZE,
        KERNEL_PHY_BASE + KERNEL_TEXT_SIZE, KERNEL_RODATA_SIZE,
        PERM_R);  // rodata
    create_mapping(rtpg_addr, KERNEL_PHY_BASE + KERNEL_TEXT_SIZE + KERNEL_RODATA_SIZE,
        KERNEL_PHY_BASE + KERNEL_TEXT_SIZE + KERNEL_RODATA_SIZE,
        KERNEL_MAPPING_SIZE - (KERNEL_TEXT_SIZE + KERNEL_RODATA_SIZE),
        PERM_R | PERM_W);  // Other Sections

    // Map User
    create_mapping(
        rtpg_addr, 0, USER_PHY_ENTRY, USER_MAPPING_SIZE, PROT_U | PERM_R | PERM_W | PERM_X);
    create_mapping(rtpg_addr, USER_STACK_TOP - USER_MAPPING_SIZE,
        USER_PHY_ENTRY + USER_MAPPING_SIZE, USER_MAPPING_SIZE, PROT_U | PERM_R | PERM_W);
}