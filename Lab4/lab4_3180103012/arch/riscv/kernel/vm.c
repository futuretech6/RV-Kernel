#include "vm.h"

/**
 * @brief Create a mapping object
 *
 * @param pgtbl 根页表的基地址
 * @param va 需要映射的虚拟地址的基地址
 * @param pa 需要映射的物理地址的基地址
 * @param sz 映射的大小
 * @param perm 映射的读写权限
 */
void create_mapping(uint64 *pgtbl, uint64 va, uint64 pa, uint64 sz, int perm) {}

void paging_init(void) {}