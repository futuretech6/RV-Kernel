#include <cstdlib>
#include <iostream>

#define PERM_R 0b10
#define PERM_W 0b100
#define PERM_X 0b1000

#define PAGE_SIZE 0x1000  // 4096 bytes

#define PAGING_DEBUG 0

typedef unsigned long uint64;
typedef unsigned char uint8;
typedef unsigned long size_t;

#define Page_Floor(__addr) ((uint64)__addr & ~(uint64)(PAGE_SIZE - 1))

#define VAtoVPN2(__va) (((uint64)(__va) >> 30) & (PAGE_SIZE - 1))
#define VAtoVPN1(__va) (((uint64)(__va) >> 21) & (PAGE_SIZE - 1))
#define VAtoVPN0(__va) (((uint64)(__va) >> 12) & (PAGE_SIZE - 1))
#define VAtoVPN(__va) ((uint64)(__va) >> 12)
#define VAtoOffset(__va) ((uint64)(__va) & (PAGE_SIZE - 1))

#define PAtoPPN(__pa) ((uint64)(__pa) >> 12)  // PPN need no division
#define PAtoOffset(__pa) ((uint64)(__pa) & (PAGE_SIZE - 1))

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

#define PTEtoPPN(__pte) (((uint64)(__pte)&0x003ffffffffffc00) >> 10)
#define PTEtoV(__pte) ((uint64)(__pte)&0x1)

int main(int argc, char const *argv[]) {
    using namespace std;
    uint64 addr = 0x80000000;
    cout << VAtoVPN2(addr) << " " << VAtoVPN1(addr) << " " << VAtoVPN0(addr) << endl;
    addr = 0xffffffe000000000;
    cout << VAtoVPN2(addr) << " " << VAtoVPN1(addr) << " " << VAtoVPN0(addr) << endl;
    addr = 0x10000000;
    cout << VAtoVPN2(addr) << " " << VAtoVPN1(addr) << " " << VAtoVPN0(addr) << endl;
    return 0;
}
