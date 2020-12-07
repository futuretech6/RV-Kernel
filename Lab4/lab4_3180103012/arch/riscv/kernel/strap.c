#include "put.h"

void put_trap_s(void) {
    static int counter = 0;
    puts("[S] Supervisor Mode Timer Interrupt ");
    puti(counter++);
    puts("\n");
    return;
}
