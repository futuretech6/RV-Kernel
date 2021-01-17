#ifndef _SCHED_H
#define _SCHED_H

#include "mm.h"
#include "types.h"

#define TASK_BASE 0xffffffe000ff0000
#define TASK_SIZE 0x1000
// #define THREAD_OFFSET (5 * 0x08)

#ifndef __ASSEMBLER__

/* task的最大数量 */
#define NR_TASKS 64

#define FIRST_TASK (task[0])
#define LAST_TASK (task[NR_TASKS - 1])

/* 定义task的状态，Lab3中task只需要一种状态。*/
#define TASK_RUNNING 0
#define TASK_INTERRUPTIBLE 1
#define TASK_UNINTERRUPTIBLE 2
#define TASK_ZOMBIE 3
#define TASK_STOPPED 4

// Ensure Mutual Exclusion
#ifdef SJF
#define PREEMPT_ENABLE 0
#else
#ifdef PRIORITY
#define PREEMPT_ENABLE 1
#else
#define PREEMPT_ENABLE 1  // PRIORITY by default
#endif

#endif

#define PREEMPT_DISABLE !PREEMPT_ENABLE

/* Lab3中进程的数量以及每个进程初始的时间片 */
#define LAB_TEST_NUM 4
#define LAB_TEST_COUNTER 5

/* 当前进程 */
extern struct task_struct *current;

/* 进程指针数组 */
extern struct task_struct *task[NR_TASKS];

/* 进程状态段数据结构 */
struct thread_struct {
    unsigned long long ra;
    unsigned long long sp;
    unsigned long long s0;
    unsigned long long s1;
    unsigned long long s2;
    unsigned long long s3;
    unsigned long long s4;
    unsigned long long s5;
    unsigned long long s6;
    unsigned long long s7;
    unsigned long long s8;
    unsigned long long s9;
    unsigned long long s10;
    unsigned long long s11;
};

/* 进程数据结构 */
struct task_struct {
    long state;     // 进程状态Lab3中进程初始化时置为TASK_RUNNING
    long counter;   // 运行剩余时间
    long priority;  // 运行优先级 1最高 5最低
    long blocked;
    long pid;  // 进程标识符
    // Above Size Cost: 40 bytes

    // uint64 sepc;
    uint64 sscratch;

    struct mm_struct *mm;
    struct thread_struct thread;  // 该进程状态段
};

/* 进程初始化 创建四个dead_loop进程 */
void task_init(void);

/* 在时钟中断处理中被调用 */
void do_timer(void);

/* 调度程序 */
void schedule(void);

/* 切换当前任务current到下一个任务next */
void switch_to(struct task_struct *next);
extern void __switch_to(struct thread_struct *prev_thread, struct thread_struct *next_thread);

#define CONTEXT_SAVE(pTask)                           \
    {                                                 \
        asm("sd ra, %0" : : "m"(pTask->thread.ra));   \
        asm("sd sp, %0" : : "m"(pTask->thread.sp));   \
        asm("sd s0, %0" : : "m"(pTask->thread.s0));   \
        asm("sd s1, %0" : : "m"(pTask->thread.s1));   \
        asm("sd s2, %0" : : "m"(pTask->thread.s2));   \
        asm("sd s3, %0" : : "m"(pTask->thread.s3));   \
        asm("sd s4, %0" : : "m"(pTask->thread.s4));   \
        asm("sd s5, %0" : : "m"(pTask->thread.s5));   \
        asm("sd s6, %0" : : "m"(pTask->thread.s6));   \
        asm("sd s7, %0" : : "m"(pTask->thread.s7));   \
        asm("sd s8, %0" : : "m"(pTask->thread.s8));   \
        asm("sd s9, %0" : : "m"(pTask->thread.s9));   \
        asm("sd s10, %0" : : "m"(pTask->thread.s10)); \
        asm("sd s11, %0" : : "m"(pTask->thread.s11)); \
    }

#define CONTEXT_LOAD(pTask)                           \
    {                                                 \
        asm("ld ra, %0" : : "m"(pTask->thread.ra));   \
        asm("ld sp, %0" : : "m"(pTask->thread.sp));   \
        asm("ld s0, %0" : : "m"(pTask->thread.s0));   \
        asm("ld s1, %0" : : "m"(pTask->thread.s1));   \
        asm("ld s2, %0" : : "m"(pTask->thread.s2));   \
        asm("ld s3, %0" : : "m"(pTask->thread.s3));   \
        asm("ld s4, %0" : : "m"(pTask->thread.s4));   \
        asm("ld s5, %0" : : "m"(pTask->thread.s5));   \
        asm("ld s6, %0" : : "m"(pTask->thread.s6));   \
        asm("ld s7, %0" : : "m"(pTask->thread.s7));   \
        asm("ld s8, %0" : : "m"(pTask->thread.s8));   \
        asm("ld s9, %0" : : "m"(pTask->thread.s9));   \
        asm("ld s10, %0" : : "m"(pTask->thread.s10)); \
        asm("ld s11, %0" : : "m"(pTask->thread.s11)); \
    }

#endif

#endif