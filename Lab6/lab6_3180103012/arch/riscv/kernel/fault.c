#include "mm.h"
#include "sched.h"
#include "slub.h"
#include "stdio.h"
#include "vm.h"

#define CAUSE_FETCH_PAGE_FAULT 12
#define CAUSE_LOAD_PAGE_FAULT 13
#define CAUSE_STORE_PAGE_FAULT 15

void do_page_fault(void) {
    uint64 bad_addr;
    asm("csrr t0, stval");
    asm("sd t0, %0" : : "m"(bad_addr));

    struct vm_area_struct *vm_area_ptr = current->mm->vm_area_list;
    for (; vm_area_ptr; vm_area_ptr = vm_area_ptr->vm_next) {
        if (vm_area_ptr->vm_start <= bad_addr && bad_addr < vm_area_ptr->vm_end)
            break;
    }
    if (!vm_area_ptr) {  // Not Found
        panic(
            "Invalid vm area in page fault - no vm area found for bad_addr=0x%lx", bad_addr);
        return;
    }

    uint64 scause;
    asm("csrr t0, scause");
    asm("sd t0, %0" : : "m"(scause));

    _Bool prot_match;
    int prot = PROT_U;
    switch (scause) {
        case CAUSE_FETCH_PAGE_FAULT:
            prot_match = (vm_area_ptr->vm_page_prot.pgprot | (prot |= PERM_X));
            break;
        case CAUSE_LOAD_PAGE_FAULT:
            prot_match = (vm_area_ptr->vm_page_prot.pgprot | (prot |= PERM_R));
            break;
        case CAUSE_STORE_PAGE_FAULT:
            prot_match = (vm_area_ptr->vm_page_prot.pgprot | (prot |= PERM_W));
            break;
        default: prot_match = 0; break;
    }
    if (!prot_match) {
        panic("Invalid vm area in page fault - prot unmatch.");
        return;
    }

    // uint64 rtpg_addr;
    // asm("csrr t0, satp");
    // asm("sd t0, %0" : : "m"(rtpg_addr));
    // rtpg_addr &= 0xfffffffffff;
    // rtpg_addr <<= PAGE_SHIFT;

    uint64 pa;

    if (bad_addr < USER_MAPPING_SIZE) {  // User Prog
        pa = USER_PHY_ENTRY + PAGE_FLOOR(bad_addr) * PAGE_SIZE;
    } else if (bad_addr >= USER_STACK_TOP - USER_MAPPING_SIZE) {  // User Stack
        pa = USER_PHY_ENTRY + USER_MAPPING_SIZE + PAGE_FLOOR(bad_addr) * PAGE_SIZE -
             (USER_STACK_TOP - USER_MAPPING_SIZE);
    } else  // Other
        pa = (uint64)VA2PA(kmalloc(PAGE_SIZE));

    create_mapping(
        current->mm->rtpg_addr, PAGE_FLOOR(bad_addr) * PAGE_SIZE, pa, PAGE_SIZE, prot);

    printf("[S] mapped PA: 0x%x to VA: 0x%lx with size=%d, prot=0x%x\n", pa,
        PAGE_FLOOR(bad_addr) * PAGE_SIZE, PAGE_SIZE, prot);
}