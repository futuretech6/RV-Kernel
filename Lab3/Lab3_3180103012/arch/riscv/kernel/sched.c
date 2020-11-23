/**
 * @file sched.c
 * @author Scott Chen
 * @brief the scheduler implementation of oslab3
 * @version 0.1
 * @date 2020-11-05
 * @ref https://gitee.com/zjuicsr/lab20fall-stu/wikis/lab3
 */
#include "sched.h"
#include "put.h"
#include "rand.h"

#define TASK_BASE 0x80010000

struct task_struct *current;
struct task_struct *task[NR_TASKS];

#define CONTEXT_SAVE(pTask)                                        \
    {                                                              \
        asm volatile("sd ra, %0" : : "m"(pTask->thread.ra));       \
        /* asm volatile("sd sp, %0" : : "m"(pTask->thread.sp)); */ \
        asm volatile("sd s0, %0" : : "m"(pTask->thread.s0));       \
        asm volatile("sd s1, %0" : : "m"(pTask->thread.s1));       \
        asm volatile("sd s2, %0" : : "m"(pTask->thread.s2));       \
        asm volatile("sd s3, %0" : : "m"(pTask->thread.s3));       \
        asm volatile("sd s4, %0" : : "m"(pTask->thread.s4));       \
        asm volatile("sd s5, %0" : : "m"(pTask->thread.s5));       \
        asm volatile("sd s6, %0" : : "m"(pTask->thread.s6));       \
        asm volatile("sd s7, %0" : : "m"(pTask->thread.s7));       \
        asm volatile("sd s8, %0" : : "m"(pTask->thread.s8));       \
        asm volatile("sd s9, %0" : : "m"(pTask->thread.s9));       \
        asm volatile("sd s10, %0" : : "m"(pTask->thread.s10));     \
        asm volatile("sd s11, %0" : : "m"(pTask->thread.s11));     \
    }

#define CONTEXT_LOAD(pTask)                                        \
    {                                                              \
        asm volatile("ld ra, %0" : : "m"(pTask->thread.ra));       \
        /* asm volatile("ld sp, %0" : : "m"(pTask->thread.sp)); */ \
        asm volatile("ld s0, %0" : : "m"(pTask->thread.s0));       \
        asm volatile("ld s1, %0" : : "m"(pTask->thread.s1));       \
        asm volatile("ld s2, %0" : : "m"(pTask->thread.s2));       \
        asm volatile("ld s3, %0" : : "m"(pTask->thread.s3));       \
        asm volatile("ld s4, %0" : : "m"(pTask->thread.s4));       \
        asm volatile("ld s5, %0" : : "m"(pTask->thread.s5));       \
        asm volatile("ld s6, %0" : : "m"(pTask->thread.s6));       \
        asm volatile("ld s7, %0" : : "m"(pTask->thread.s7));       \
        asm volatile("ld s8, %0" : : "m"(pTask->thread.s8));       \
        asm volatile("ld s9, %0" : : "m"(pTask->thread.s9));       \
        asm volatile("ld s10, %0" : : "m"(pTask->thread.s10));     \
        asm volatile("ld s11, %0" : : "m"(pTask->thread.s11));     \
    }

/**
 * @brief init tasks, create 4 threads running dead-loop
 */
void task_init(void) {
    current = (struct task_struct *)TASK_BASE;
    for (int i = 0; i <= LAB_TEST_NUM; i++) {
        task[i]        = (struct task_struct *)(long)(TASK_BASE + TASK_SIZE * i);
        task[i]->state = TASK_RUNNING;
        if (i == 0)
            task[i]->counter = 0;
        else {
#if PREEMPT_ENABLE == 0  // SJF
            task[i]->counter = rand();
#else
            task[i]->counter = 8 - i;
#endif
        }
        task[i]->priority  = 5;
        task[i]->blocked   = 0;
        task[i]->pid       = i;
        task[i]->thread.sp = TASK_BASE + TASK_SIZE * (i + 1);
        asm volatile("la t0, dead_loop");
        asm volatile("sd t0, %0" ::"m"(task[i]->thread.ra));
        if (i != 0) {
            puts("[PID = ");
            puti(task[i]->pid);
            puts("] Process Create Successfully! counter = ");
            puti(task[i]->counter);
#if PREEMPT_ENABLE == 1  // PRIORITY
            puts(" priority = ");
            puti(task[i]->priority);
#endif
            puts("\n");
        }
    }
}

/**
 * @brief call by timer int
 */
void do_timer(void) {
#if PREEMPT_ENABLE == 0  // SJF
    // Print thread info for SJF
    puts("[PID = ");
    puti(current->pid);
    puts("] ");
    puts("Context Calculation: ");
    puts("counter = ");
    puti(current->counter);
    puts("\n");

    // Decrease counter and schedule
    current->counter--;
    if (current->counter <= 0)
        schedule();

#else  // PRIORITY
    current->counter--;
    schedule();
    if (current->counter <= 0)
        current->counter = (current->pid == 0) ? 5 : 8 - current->pid;
#endif
}

/**
 * @brief schedule implementation
 */
void schedule(void) {
#if PREEMPT_ENABLE == 0               // SJF
    int i_min_cnt    = LAB_TEST_NUM;  // index of min but not zero counter
    _Bool all_zeroes = 1;
    for (int i = LAB_TEST_NUM; i > 0; i--) {
        if (task[i]->state == TASK_RUNNING) {
            if (task[i]->counter != 0 && task[i]->counter < task[i_min_cnt]->counter ||
                task[i_min_cnt]->counter == 0)
                i_min_cnt = i;
            if (task[i]->counter)
                all_zeroes = 0;
        }
    }
    if (all_zeroes) {
        for (int i = 1; i <= LAB_TEST_NUM; i++) {
            if (task[i]->state == TASK_RUNNING) {
                task[i]->counter = rand();
                puts("[PID = ");
                puti(task[i]->pid);
                puts("] Reset counter = ");
                puti(task[i]->counter);
                puts("\n");
            }
        }
        schedule();
    } else {
        switch_to(task[i_min_cnt]);
    }

#else  // PRIORITY
    int max_pri = __INT_MAX__, i_min_cnt = 1;
    for (int i = 1; i <= LAB_TEST_NUM; i++) {
        if (task[i]->state == TASK_RUNNING) {
            if (task[i]->priority < max_pri) {
                i_min_cnt = i;
                max_pri = task[i]->priority;
            } else if (task[i]->priority == max_pri &&
                       task[i]->counter < task[i_min_cnt]->counter && task[i]->counter > 0)
                i_min_cnt = i;
        }
    }

    // Must be printed here to meet demands, else the printed info is out-dated
    puts("[!] Switch from task ");
    puti(current->pid);
    puts(" to task ");
    puti(task[i_min_cnt]->pid);
    puts(", prio: ");
    puti(task[i_min_cnt]->priority);
    puts(", counter: ");
    puti(task[i_min_cnt]->counter);
    puts("\n");

    // Use another loop to update prio
    for (int i = 1; i <= LAB_TEST_NUM; i++)
        if (task[i]->state == TASK_RUNNING)
            task[i]->priority = rand();

    switch_to(task[i_min_cnt]);
#endif
}

/**
 * @brief context switch from current to next
 */
void switch_to(struct task_struct *next) {
    if (current == next)
        return;

#if PREEMPT_ENABLE == 0  // SJF
    puts("[!] Switch from task ");
    puti(current->pid);
    puts(" to task ");
    puti(next->pid);
    puts(", prio: ");
    puti(next->priority);
    puts(", counter: ");
    puti(next->counter);
    puts("\n");
#endif

    asm volatile("mv t0, %0" ::"r"(&current));       // t0 = &current
    asm volatile("mv t1, %0" ::"r"(&next));          // t1 = &next
    asm volatile("mv t4, %0" ::"r"(THREAD_OFFSET));  // t4 = t0 = &current

    switch_to_asm();
    // CONTEXT_SAVE(current);
    // current = next;
    // CONTEXT_LOAD(current);  // This `current` is the argv `next`

#if PREEMPT_ENABLE == 1  // PRIORITY
    // Print all threads' info for PRIORITY
    puts("tasks' priority changed\n");
    for (int i = 1; i <= LAB_TEST_NUM; i++) {
        if (task[i]->state == TASK_RUNNING) {
            puts("[PID = ");
            puti(task[i]->pid);
            puts("] ");
            puts("counter = ");
            puti(task[i]->counter);
            puts(" priority = ");
            puti(task[i]->priority);
            puts("\n");
        }
    }
#endif
}

/**
 * @brief dead loop
 */
void dead_loop(void) {
    for (;;)
        ;
}
