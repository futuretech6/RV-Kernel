#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "arch/riscv/include/vm.h"

int main(int argc, char const *argv[]) {
    using namespace std;

    // L2
    uint64 VA = 0x80000ffc;
    printf("0x%lx 0x%lx\n", VAtoVPN2(VA), 0x80008000 + VAtoVPN2(VA) * 8);

    // L1
    uint64 pte2 = 0x20002c01;
    printf("0x%lx 0x%lx\n", PTEtoPPN(pte2), (PTEtoPPN(pte2) << 12) + VAtoVPN1(VA) * 8);

    // L0
    uint64 pte1 = 0x20003001;
    printf("0x%lx 0x%lx\n", PTEtoPPN(pte1), (PTEtoPPN(pte1) << 12) + VAtoVPN0(VA) * 8);

    // Phy
    uint64 pte0 = 0x2000000f;
    printf("0x%lx 0x%lx\n", PTEtoPPN(pte0), (PTEtoPPN(pte0) << 12) + (VA & 0xfff));

    return 0;
}
