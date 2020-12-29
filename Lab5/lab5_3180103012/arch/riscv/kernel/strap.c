/**
 * @file strap.c
 * @author Scott Chen
 * @brief the implementation S trap handling of oslab4
 * @version 0.2
 * @date 2020-12-05
 * @ref https://gitee.com/zjuicsr/lab20fall-stu/wikis/lab5
 */
#include "put.h"
#include "sched.h"
#include "syscall.h"
#include "types.h"

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

extern struct task_struct *current;

size_t handler_sys_write(unsigned int fd, const char *buf, size_t count) {
    size_t cnt = 0;
    if (fd == 1)
        for (cnt = 0; buf[cnt] != 0 && cnt < count; cnt++)
            *UART_VIR_ADDR = (unsigned char)buf[cnt];
    return cnt;
}

long handler_sys_getpid(void) {
    return current->pid;
}

void handler_s(uint64 scause, uint64 sepc, uint64 *regs) {
    uint64 syscall_id    = regs[17];   // a7
    uint64 *syscall_argv = &regs[10];  // a0 ~ a6
    uint64 *syscall_ret  = &regs[10];  // a0 ~ a1

    switch (syscall_id) {
        case SYS_WRITE_ID:
            syscall_ret[0] = handler_sys_write((unsigned int)syscall_argv[0],
                (char *)syscall_argv[1], (size_t)syscall_argv[2]);
            break;
        case SYS_GETPID_ID: syscall_ret[0] = handler_sys_getpid(); break;
        default: break;
    }
}
