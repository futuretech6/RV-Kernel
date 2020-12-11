#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "arch/riscv/include/vm.h"

int main(int argc, char const *argv[]) {
    using namespace std;
    // L2
    uint64 addr = 0xffffffe0000002ec;
    // uint64 addr = 0x80000000;
    printf("0x%lx 0x%lx\n", VAtoVPN2(addr), 0x80008000 + VAtoVPN2(addr) * 8);

    // L1
    uint64 pte = 0x20004801;
    printf("0x%lx 0x%lx\n", PTEtoPPN(pte), PTEtoPPN(pte) << 12 + VAtoVPN1(addr) * 8);
    return 0;
}
