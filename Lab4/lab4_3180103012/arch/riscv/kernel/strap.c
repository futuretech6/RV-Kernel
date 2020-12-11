/**
 * @file strap.c
 * @author Scott Chen
 * @brief the implementation S trap handling of oslab4
 * @version 0.2
 * @date 2020-12-05
 * @ref https://gitee.com/zjuicsr/lab20fall-stu/wikis/lab4
 */
#include "put.h"

void strap_TimerInt(void) {
    static int counter = 0;
    puts("[S] Supervisor Mode Timer Interrupt ");
    putd(counter++);
    puts("\n");
    return;
}

void strap_instPF(void) {
    puts("[S] Supervisor Mode Page Fault Exception While Reading Instructions\n");
    return;
}

void strap_loadPF(void) {
    puts("[S] Supervisor Mode Page Fault Exception While Loading Data\n");
    return;
}

void strap_storePF(void) {
    puts("[S] Supervisor Mode Page Fault Exception While Storing Data\n");
    return;
}