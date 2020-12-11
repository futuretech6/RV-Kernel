#include "test.h"
#include "put.h"
#include "sched.h"

int os_test() {
    puts("ZJU OS LAB 4             GROUP-17\ntask init...\n");

    task_init();
    dead_loop();

    return 0;
}
