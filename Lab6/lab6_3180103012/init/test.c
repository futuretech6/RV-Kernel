#include "test.h"
#include "put.h"
#include "sched.h"

int os_test() {
    puts("ZJU OS LAB 5             3180103012/GROUP-17\n");
    puts("task init...\n");

    task_init();
    for (;;)
        ;

    return 0;
}
