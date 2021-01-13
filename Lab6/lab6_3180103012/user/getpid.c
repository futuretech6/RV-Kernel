#include "syscall.h"
#include "stdio.h"

register void* current_sp __asm__("sp");

static inline long getpid() {
  long ret;
  asm volatile ("li a7, %1\n"
                "ecall\n"
                "mv %0, a0\n"
                : "+r" (ret) : "i" (SYS_GETPID));
  return ret;
}

static inline long fork() {
  long ret;
  asm volatile ("li a7, %1\n"
                "ecall\n"
                "mv %0, a0\n"
                : "+r" (ret) : "i" (SYS_FORK));
  return ret;
}

int main() {
  long ret;
  ret = fork();
  ret = fork();
  while (1) {
    printf("[User] pid: %ld, sp is %lx\n", getpid(), current_sp);
    for (unsigned int i = 0; i < 0xFFFFFFFF; i++);
  }
  
  return 0;
}
