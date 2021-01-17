#include "mm.h"
#include "sched.h"
#include "slub.h"
#include "stddef.h"
#include "types.h"
#include "vm.h"

#define LoadPTE(__pte_addr, __prot, __v)                               \
    {                                                                  \
        *__pte_addr = ((uint64)(*(__pte_addr)) & 0xfffffffffffffc00) | \
                      ((uint64)(__prot) | (uint64)(__v));              \
    }

unsigned long get_unmapped_area(size_t length) {
    if (!current->mm->vm_area_list)  // Not allocated yet
        return 0x0L;

    for (unsigned long addr = 0;; addr += PAGE_SIZE) {
        struct vm_area_struct *p;
        for (p = current->mm->vm_area_list; p->vm_next; p = p->vm_next) {
            if (p->vm_start < addr && addr < p->vm_end ||
                p->vm_start < addr + length && addr + length < p->vm_end) {
                break;
            }
        }
        if (p->vm_next == NULL) {  // Traverse to the last in vm_area_list with no conflict
            return addr;
        }
    }
    // panic("No enough space");
}

void *do_mmap(struct mm_struct *mm, void *start, size_t length, int prot, unsigned long flags,
    unsigned long pgoff) {

    _Bool isCollision = 0;
    if (mm->vm_area_list)  // vm_area_list not empty
        for (struct vm_area_struct *p = mm->vm_area_list; p->vm_next; p = p->vm_next)
            if (p->vm_start <= start && start < p->vm_end ||
                p->vm_start < start + length && start + length <= p->vm_end) {
                isCollision = 1;
                break;
            }

    if (isCollision)  // Collision
        start = (void *)get_unmapped_area(length);

    struct vm_area_struct *vm_area_ptr = kmalloc(sizeof(struct vm_area_struct));

    vm_area_ptr->vm_start            = start;
    vm_area_ptr->vm_end              = start + length;
    vm_area_ptr->vm_next             = NULL;
    vm_area_ptr->vm_mm               = mm;
    vm_area_ptr->vm_page_prot.pgprot = prot;
    vm_area_ptr->vm_flags            = flags;

    if (!mm->vm_area_list) {  // Empty vm area list
        vm_area_ptr->vm_prev = NULL;
        mm->vm_area_list     = vm_area_ptr;
    } else {  // Traverse existing vm area list
        struct vm_area_struct *p;
        for (p = mm->vm_area_list; p->vm_next; p = p->vm_next)
            ;
        p->vm_next           = vm_area_ptr;
        vm_area_ptr->vm_prev = p;
    }

    return start;
}

/**
 * @param __addr 建议映射的虚拟首地址，需要按页对齐
 * @param __len 映射的长度，需要按页对齐
 * @param __prot 映射区的权限，在4.3.1节中已说明
 * @param __flags 由于本次实验不涉及mmap在Linux中的其他功能，该参数无意义，固定为
 * (MAP_PRIVATE | MAP_ANONYMOUS)
 * @param __fd 由于本次实验不涉及文件映射，该参数无意义，固定为-1
 * @param __offset 由于本次实验不涉及文件映射，该参数无意义，固定为0
 * @return void* 实际映射的虚拟首地址，若虚拟地址区域[__addr, __addr+__len)未与
 * 其它地址映射冲突，则返回值就是建议首地址__addr，若发生冲突，则需要更换到无冲突的虚拟地址区域，返回值为该区域的首地址
 */
void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset) {
    return do_mmap(current->mm, __addr, __len, __prot, MAP_PRIVATE | MAP_ANONYMOUS, 0);
}

void free_page_tables(uint64 pagetable, uint64 va, uint64 n, int free_frame) {
    for (uint64 addr_last_byte = va + n - 1; va <= addr_last_byte; va += PAGE_SIZE) {
        LoadPTE(page_walk(pagetable, va), 0, 1);
    }
}

/**
 * @brief
 *
 * @param start
 * @param length
 * @return int
 */
int munmap(void *start, size_t length) {
    _Bool found = 0;
    for (struct vm_area_struct *p = current->mm->vm_area_list; p; p = p->vm_next) {
        if (p->vm_start >= start && p->vm_end <= start + length) {
            free_page_tables(current->mm->rtpg_addr, start, PAGE_CEIL(length), 1);
            if (p->vm_prev)
                p->vm_prev->vm_next = p->vm_next;
            if (p->vm_next)
                p->vm_next->vm_prev = p->vm_prev;
            kfree(p);
            found = 1;
        }
    }
    return (found ? 0 : -1);
}

/**
 * @brief
 *
 * @param __addr
 * @param __len
 * @param __prot
 * @return int
 */
int mprotect(void *__addr, size_t __len, int __prot) {
    for (uint64 addr_last_byte = __addr + __len - 1; __addr <= addr_last_byte;
         __addr += PAGE_SIZE) {
        LoadPTE(page_walk(current->mm->rtpg_addr, __addr), __prot, 1);
    }
}