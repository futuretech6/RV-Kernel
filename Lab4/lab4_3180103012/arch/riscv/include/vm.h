#define NULL 0x0

#define PHY_BASE 0x80000000
#define VIR_BASE 0xffffffe000000000
#define MAPPING_SIZE 0x1000000  // 16MB

#define PAGE_SIZE 0x1000      // 4096 bytes
#define PAGE_ENTRY_NUM 0x200  // 512

#define FREE_SPACE_SIZE 0x800000  // [rt_pg_addr, rt_pg_addr + limit): 8MB

#define TEXT_SIZE 0x2000
#define RODATA_SIZE 0x1000

#define PERM_R 0b10
#define PERM_W 0b100
#define PERM_X 0b1000

#define PAGING_DEBUG 0

typedef unsigned long uint64;
typedef unsigned char uint8;
typedef unsigned long size_t;

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

#define Page_Floor(__addr) ((uint64)(__addr) & ~(uint64)(PAGE_SIZE - 1))

#define VAtoVPN2(__va) (((uint64)(__va) >> 30) & (PAGE_ENTRY_NUM - 1))
#define VAtoVPN1(__va) (((uint64)(__va) >> 21) & (PAGE_ENTRY_NUM - 1))
#define VAtoVPN0(__va) (((uint64)(__va) >> 12) & (PAGE_ENTRY_NUM - 1))

#define PAtoPPN(__pa) (((uint64)(__pa) >> 12) & 0xfffffffffff)  // PPN need no division

// PROT = {RSW, D, A, G, U, X, W, R, V} = {6'b0, PERM_X|W|R, V}
#define LoadPTE(__pte_addr, __ppn, __perm, __v)                                     \
    {                                                                               \
        *__pte_addr = ((uint64)(*(__pte_addr)) & 0xffc0000000000000) |              \
                      ((uint64)(__ppn) << 10) | ((uint64)(__perm) | (uint64)(__v)); \
    }

#define PTEtoPPN(__pte) (((uint64)(__pte) >> 10) & 0xfffffffffff)
#define PTEtoV(__pte) ((_Bool)((uint64)(__pte)&0x1))

void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm);
void paging_init(void);