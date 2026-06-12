#include "mm.h"
#include "sched.h"
#include "entry.h"

int copy_process(unsigned long fn, unsigned long arg) {
    disable_preempt();

    struct task_struct *p = (struct task_struct *)get_free_page();
    if (!p) return 1;

    p->priority = current->priority;
    p->state = RUNNING;
    p->counter = p->priority;
    p->preempt_count = 1;

    p->cpu_context.x19 = fn;
    p->cpu_context.x20 = arg;
    p->cpu_context.pc = (unsigned long)ret_from_fork;
    p->cpu_context.sp = (unsigned long)p + THREAD_SIZE; 

    // add to the scheduler
    int pid = nr_tasks++;
    p->pid = pid;
    task[pid] = p;
    
    enable_preempt();
    return 0;
}
