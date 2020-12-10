#define NULL 0x0

#define PERM_R 0b10
#define PERM_W 0b100
#define PERM_X 0b1000

#define PAGE_SIZE 4096

typedef unsigned long uint64;
typedef unsigned char uint8;
typedef unsigned long size_t;

struct free_list_node {
    void *base;
    size_t limit;
    struct free_list_node *next;
};

/**
 * @brief
 *   63      54 53        28 27        19 18        10 9    8 7 6 5 4 3 2 1 0
 *   -----------------------------------------------------------------------
 * | Reserved |   PPN[2]   |   PPN[1]   |   PPN[0]   | RSW |D|A|G|U|X|W|R|V|
 *  -----------------------------------------------------------------------
 *
 */
struct pageTable {
    uint64 PTE_list[512];
};

#define Page_Floor(__addr) ((uint64)__addr & ~(PAGE_SIZE - 1))

#define VAtoVPN2(__va) (((uint64)(__va) >> 30) % PAGE_SIZE)
#define VAtoVPN1(__va) (((uint64)(__va) >> 21) % PAGE_SIZE)
#define VAtoVPN0(__va) (((uint64)(__va) >> 12) % PAGE_SIZE)
#define VAtoVPN(__va) ((uint64)(__va) >> 12)
#define VAtoOffset(__va) ((uint64)(__va) % 0x1000)

// #define PAtoPPN2(__pa) (((__pa) >> 30) % 0x4000000)
// #define PAtoPPN1(__pa) (((__pa) >> 21) % PAGE_SIZE)
// #define PAtoPPN0(__pa) (((__pa) >> 12) % PAGE_SIZE)

#define PAtoPPN(__pa) ((uint64)(__pa) >> 12)
#define PAtoOffset(__pa) ((uint64)(__pa) % 0x1000)

// #define PPN2toPTE(__pte, __ppn2) \
//     { __pte = ((__pte)&0xffc000000fffffff) | ((__ppn2) << 28); }
// #define PPN1toPTE(__pte, __ppn1) \
//     { __pte = ((__pte)&0xfffffffff007ffff) | ((__ppn1) << 19); }
// #define PPN0toPTE(__pte, __ppn0) \
//     { __pte = ((__pte)&0xfffffffffff803ff) | ((__ppn0) << 10); }

#define PPNtoPTE(__pte, __ppn) \
    { __pte = ((uint64)(__pte)&0xffc0000000003ff) | ((uint64)(__ppn) << 10); }
// {RSW, D, A, G, U, X, W, R, V} = {6'b0, PERM_X|W|R, V}
#define PROTtoPTE(__pte, __perm, __v) \
    { __pte = ((uint64)(__pte)&0xfffffffffffffc00) | ((uint64)(__perm) | (uint64)(__v)); }
#define LoadPTE(__pte, __ppn, __perm, __v) \
    {                                      \
        PPNtoPTE(__pte, __ppn);            \
        PROTtoPTE(__pte, __perm, __v);     \
    }

#define PTEtoPA(__pte) (((__pte)&0x003ffffffffffc00) >> 10)
#define PTEtoV(__pte) ((__pte) % 0b10)

// extern struct pageTable *pRootPT;

int create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm);
void paging_init(void);