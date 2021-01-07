#pragma once

// pages
#define PHY_START 0x80000000L
#define PHY_END (PHY_START + 16 * 1024 * 1024)

#define PAGE_SIZE 4096L
#define PAGE_SHIFT 12

#define PAGE_UP(addr) ((((uint64)addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))
#define PAGE_DOWN(addr) ((((uint64)addr)) & ~(PAGE_SIZE - 1))

#define PTE_V (1L << 0)
#define PTE_R (1L << 1)
#define PTE_W (1L << 2)
#define PTE_X (1L << 3)
#define PTE_U (1L << 4)
#define PTE_G (1L << 5)
#define PTE_A (1L << 6)
#define PTE_D (1L << 7)

#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)
#define PTE2PA(pte) (((pte) >> 10) << 12)
#define PTE_FLAGS(pte) ((pte)&0x3FF)

#define PAGE_OFFSET 0xffffffe000000000L
#define VA_PA_OFFSET ((uint64)PAGE_OFFSET - (uint64)PHY_START)
#define VA2PA(va) ((uint64)va - (uint64)VA_PA_OFFSET)
#define PA2VA(pa) ((uint64)pa + (uint64)VA_PA_OFFSET)

#define PXMASK 0x1FF
#define PXSHIFT(level) (PAGE_SHIFT + (9 * (level)))
#define PX(level, va) ((((uint64)(va)) >> PXSHIFT(level)) & PXMASK)

#define SATP_SV39 (8L << 60)
#define MAKE_SATP(pagetable) (SATP_SV39 | (((uint64)pagetable) >> 12))

#define PAGE_ALLOC 1
#define NO_PAGE_ALLOC 0

#define write_csr(reg, val) ({ asm volatile("csrw " #reg ", %0" ::"rK"(val)); })
#define read_csr(reg)                                 \
    ({                                                \
        unsigned long __tmp;                          \
        asm volatile("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp;                                        \
    })
