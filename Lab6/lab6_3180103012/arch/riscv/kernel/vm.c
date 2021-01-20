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
#include "buddy.h"
#include "mm.h"
#include "slub.h"
#include "stdio.h"
#include "string.h"

#define Page_Floor(__addr) ((uint64)(__addr) & ~(uint64)(PAGE_SIZE - 1))

#define VAtoVPN2(__va) (((uint64)(__va) >> 30) & (PAGE_ENTRY_NUM - 1))
#define VAtoVPN1(__va) (((uint64)(__va) >> 21) & (PAGE_ENTRY_NUM - 1))
#define VAtoVPN0(__va) (((uint64)(__va) >> 12) & (PAGE_ENTRY_NUM - 1))

#define PAtoPPN(__pa) (((uint64)(__pa) >> 12) & 0xfffffffffff)  // PPN need no division

// PROT = {RSW, D, A, G, U, X, W, R, V} = {6'b0, PERM_X|W|R, V}
#define LoadPTE(__pte_addr, __ppn, __prot, __v)                                     \
    {                                                                               \
        *__pte_addr = ((uint64)(*(__pte_addr)) & 0xffc00000000003fe) |              \
                      ((uint64)(__ppn) << 10) | ((uint64)(__prot) | (uint64)(__v)); \
    }

#define PTEtoPPN(__pte) (((uint64)(__pte) >> 10) & 0xfffffffffff)
#define PTEtoV(__pte) ((_Bool)((uint64)(__pte)&0x1))

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
        asm("la t0, kernel_rt_pg_addr");
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

void kfree_byte(void *addr) {}

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
            if ((pgtbl = (uint64 *)VA2PA(alloc_pages(1))) == NULL) {
                // if ((pgtbl = (uint64 *)kmalloc(PAGE_SIZE)) == NULL) {
                puts("\n[!] Insufficient Free Space.\n");
                return NULL;  // Insufficient free space for pg tbls
            }
            memset(pgtbl, 0, PAGE_SIZE);
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
 * @brief Map kernel space to equal addr and higer addr, and map hardware address
 */
void kernel_paging_init(void) {
    init_buddy_system();

    uint64 *rtpg_addr = (uint64 *)VA2PA(alloc_pages(1));

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
}
