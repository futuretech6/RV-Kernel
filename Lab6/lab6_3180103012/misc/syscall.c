/**
 * @file syscall.c
 * @author Scott Chen
 * @brief My Implementation of the API of some syscalls, need no to be compiled in this lab
 * @version 0.1
 * @date 2020-12-14
 */
#include "syscall.h"
#include "put.h"
#include "sched.h"

/**
 * @brief syscall_id=64
 *
 * @param fd file descriptor, 1 for stdout
 * @param buf address where print starts
 * @param count max length of string
 * @return size_t, number of chars that being printed
 */
size_t sys_write(unsigned int fd, const char *buf, size_t count) {
    size_t retVal;
    asm("li a7, %0" ::"n"(SYS_WRITE_ID));
    asm("ld a0, %0" ::"m"(fd));
    asm("ld a1, %0" ::"m"(buf));
    asm("ld a2, %0" ::"m"(count));
    asm("ecall");
    asm("sd a0, %0" ::"m"(retVal));
    return retVal;
}

/**
 * @brief syscall_id=172
 *
 * @return pid_t, current thread's pid
 */
pid_t sys_getpid(void) {
    pid_t retVal;
    asm("li a7, %0" ::"n"(SYS_GETPID_ID));
    asm("ecall");
    asm("sd a0, %0" ::"m"(retVal));
    return retVal;
}
