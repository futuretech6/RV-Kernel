#ifndef _MM_H
#define _MM_H

#include "types.h"

// #define PROT_NONE 0x0   // 页内容不可被访问
// #define PROT_READ 0x1   // 页内容可以被读取
// #define PROT_WRITE 0x2  // 页可以被写入内容
// #define PROT_EXEC 0x4   // 页内容可以被执行
#define PROT_SEM 0x8  // (可选) 页面可能用于原子操作(atomic operation)
#define PROT_GROWSDOWN 0x01000000
#define PROT_GROWSUP 0x02000000

#define MAP_PRIVATE 0x2
#define MAP_ANONYMOUS 0x20

typedef struct {
    unsigned long pgprot;
} pgprot_t;

struct vm_area_struct {
    /* Our start address within vm_area. */
    unsigned long vm_start;
    /* The first byte **after** our end address within vm_area. */
    unsigned long vm_end;
    /* linked list of VM areas per task, sorted by address. */
    struct vm_area_struct *vm_next, *vm_prev;
    /* The address space we belong to. */
    struct mm_struct *vm_mm;
    /* Access permissions of this VMA. */
    pgprot_t vm_page_prot;
    /* Flags*/
    unsigned long vm_flags;
};

struct mm_struct {
    uint64 *rtpg_addr;
    struct vm_area_struct *vm_area_list;
};

void *do_mmap(struct mm_struct *mm, void *start, size_t length, int prot, unsigned long flags,
    unsigned long pgoff);
void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset);
int munmap(void *start, size_t length);
int mprotect(void *__addr, size_t __len, int __prot);

#endif