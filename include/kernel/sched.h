#define THREAD_CPU_CONTEXT  0   // offset of cpu_context

#ifndef __ASSEMBLER__

#define THREAD_SIZE         4096
#define NR_TASKS			64 

struct cpu_context {
    unsigned long x19;
    unsigned long x20;
    unsigned long x21;
    unsigned long x22;
    unsigned long x23;
    unsigned long x24;
    unsigned long x25;
    unsigned long x26;
    unsigned long x27;
    unsigned long x28;
    unsigned long fp;
    unsigned long sp;
    unsigned long pc;
};

enum proc_state { UNUSED, BLOCKED, RUNNABLE, RUNNING, ZOMBIE };

struct task_struct {
    struct cpu_context cpu_context;
    enum proc_state state;
    long pid;
    long counter;
    long priority;
    long preempt_count;
};

extern struct task_struct init_task;
extern struct task_struct* current;
extern struct task_struct* task[NR_TASKS];
extern int nr_tasks;

void print_proc(struct task_struct* addr);

void disable_preempt();
void enable_preempt();
void schedule();
void sched_timer_tick();
void switch_to(struct task_struct* next);
void cpu_switch_to(struct task_struct* prev, struct task_struct* next);
#endif
