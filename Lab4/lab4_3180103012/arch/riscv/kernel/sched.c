/**
 * @file sched.c
 * @author Scott Chen
 * @brief the scheduler implementation of oslab4
 * @version 0.2
 * @date 2020-12-05
 * @ref https://gitee.com/zjuicsr/lab20fall-stu/wikis/lab4
 */
#include "sched.h"
#include "put.h"
#include "rand.h"

struct task_struct *current;
struct task_struct *task[NR_TASKS];

/**
 * @brief init tasks, create 4 threads running dead-loop
 */
void task_init(void) {
    current = (struct task_struct *)TASK_BASE;
    for (int i = 0; i <= LAB_TEST_NUM; i++) {
        task[i]           = (struct task_struct *)(long)(TASK_BASE + TASK_SIZE * i);
        task[i]->state    = TASK_RUNNING;
        task[i]->counter  = i == 0 ? 0 : (PREEMPT_ENABLE ? 8 - i : rand());
        task[i]->priority = 5;
        task[i]->blocked  = 0;
        task[i]->pid      = i;

        task[i]->thread.sp = TASK_BASE + TASK_SIZE * (i + 1) - 0x1;
        asm("la t0, thread_init");
        asm("sd t0, %0" ::"m"(task[i]->thread.ra));

        if (i != 0) {
            puts("[PID = ");
            putd(task[i]->pid);
            puts("] Process Create Successfully! counter = ");
            putd(task[i]->counter);
#if PREEMPT_ENABLE == 1  // PRIORITY
            puts(" priority = ");
            putd(task[i]->priority);
#endif
            puts("\n");
        }
    }
}

/**
 * @brief called by timer int
 */
void do_timer(void) {
#if PREEMPT_ENABLE == 0  // SJF
    // Print thread info for SJF
    puts("[PID = ");
    putd(current->pid);
    puts("] ");
    puts("Context Calculation: ");
    puts("counter = ");
    putd(current->counter);
    puts("\n");

    // Decrease counter and schedule
    current->counter--;
    if (current->counter <= 0)
        schedule();

#else  // PRIORITY
    current->counter--;
    if (current->counter <= 0)
        current->counter = (current->pid == 0) ? 5 : 8 - current->pid;
    schedule();
#endif
}

/**
 * @brief context switch from current to next
 *
 * @param next
 */
void switch_to(struct task_struct *next) {
    if (current == next)
        return;

    asm("addi sp, sp, 32");  // Restore the stack of switch_to
    CONTEXT_SAVE(current);   // Do context save
    current = next;          // `next` in $s0(-O0), will be overwrite soon
    CONTEXT_LOAD(current);   // This `current` is the argv `next`
    asm("ret");
}

/**
 * @brief dead loop
 */
void dead_loop(void) {
    for (;;)
        ;
}

/**
 * @brief schedule implementation
 */
void schedule(void) {
#if PREEMPT_ENABLE == 0               // SJF
    int i_min_cnt    = LAB_TEST_NUM;  // index of min but not zero counter
    _Bool all_zeroes = 1;
    for (int i = LAB_TEST_NUM; i > 0; i--)
        if (task[i]->state == TASK_RUNNING) {
            if (task[i]->counter > 0 && task[i]->counter < task[i_min_cnt]->counter ||
                task[i_min_cnt]->counter == 0)
                i_min_cnt = i;
            if (task[i]->counter > 0)  // In case of negative cnt
                all_zeroes = 0;
        }
    if (all_zeroes) {
        for (int i = 1; i <= LAB_TEST_NUM; i++)
            if (task[i]->state == TASK_RUNNING) {
                task[i]->counter = rand();

                puts("[PID = ");
                putd(task[i]->pid);
                puts("] Reset counter = ");
                putd(task[i]->counter);
                puts("\n");
            }
        schedule();
    } else {
        PRINT_SWITCH_INFO();

        switch_to(task[i_min_cnt]);
    }

#else  // PRIORITY
    int max_pri = __INT_MAX__, i_min_cnt = 1;
    for (int i = 1; i <= LAB_TEST_NUM; i++)
        if (task[i]->state == TASK_RUNNING)
            if (task[i]->priority < max_pri) {
                i_min_cnt = i;
                max_pri   = task[i]->priority;
            } else if (task[i]->priority == max_pri &&
                       task[i]->counter < task[i_min_cnt]->counter && task[i]->counter > 0)
                i_min_cnt = i;

    // Must be printed here to meet demands, else the printed info is out-dated
    PRINT_SWITCH_INFO();

    // Use another loop to update prio
    for (int i = 1; i <= LAB_TEST_NUM; i++)
        if (task[i]->state == TASK_RUNNING)
            task[i]->priority = rand();

    // Print all threads' info for PRIORITY
    puts("tasks' priority changed\n");
    for (int i = 1; i <= LAB_TEST_NUM; i++)
        if (task[i]->state == TASK_RUNNING) {
            puts("[PID = ");
            putd(task[i]->pid);
            puts("] ");
            puts("counter = ");
            putd(task[i]->counter);
            puts(" priority = ");
            putd(task[i]->priority);
            puts("\n");
        }
    switch_to(task[i_min_cnt]);
#endif
}
