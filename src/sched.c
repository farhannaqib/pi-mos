#include "sched.h"
#include "io.h"
#include "irq.h"

struct task_struct init_task = {
    .cpu_context = {2,1,2,0,1,0,0,0,0,0,0,0,0},
    .state = RUNNING,
    .pid = 0,
    .counter = 5,
    .priority = 1,
    .preempt_count = 0
};

struct task_struct *current = &init_task;
struct task_struct *task[NR_TASKS] = {&init_task};
int nr_tasks = 1;

void disable_preempt() {
	current->preempt_count++;
}

void enable_preempt() {
	current->preempt_count--;
} 

void print_proc(struct task_struct* addr) {
	uart_write_text("\nPID:");
	uart_write_char(addr->pid+'0');
	uart_write_text(" State: ");
	uart_write_char(addr->state+'0');
	uart_write_text(" counter: ");
	uart_write_char(addr->counter+'0');
	uart_write_text(" priority: ");
	uart_write_char(addr->priority+'0');
	uart_write_text(" preempt: ");
	uart_write_char(addr->preempt_count+'0');
	uart_write_text("\n");
}

void schedule() {
	int next,c;
	struct task_struct * p;
	// uart_write_text("currently on: ");
	// uart_write_char(current->pid + '0');
	// uart_write_char('\n');
	// print_proc(task[0]);
	// print_proc(task[1]);
	// print_proc(task[2]);
	while (1) {
		c = -1;
		next = 0;
		for (int i = 0; i < NR_TASKS; i++){
			p = task[i];
			if (p && (p->state == RUNNING || p->state == RUNNABLE) && p->counter > c) {
				c = p->counter;
				next = i;
			}
		}
		if (c) {
			break;
		}
		for (int i = 0; i < NR_TASKS; i++) {
			p = task[i];
			if (p) {
				p->counter = (p->counter >> 1) + p->priority;
			}
		}
	}
	// uart_write_text("Switching to: ");
	// uart_write_char('0' + next);
	switch_to(task[next]);
    // TODO enable/disable preempts
}

void switch_to(struct task_struct* next) {
	if (current == next) return;
	print_proc(next);
	struct task_struct * prev = current;
	current = next;
	cpu_switch_to(prev, next);
}

void sched_timer_tick() {
    current->counter--;
    if (current->counter > 0 || current->preempt_count > 0) {
		return;
	}
    current->counter = 0;
	enable_irq();
	schedule();
	disable_irq();
}
