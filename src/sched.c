#include "sched.h"
#include "io.h"
#include "irq.h"

struct task_struct init_task = {
    .cpu_context = {0,0,0,0,0,0,0,0,0,0,0,0,0},
    .state = RUNNING,
    .pid = 0,
    .counter = 0,
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
	uart_write_text("PID:");
	uart_write_int(addr->pid);
	uart_write_text(" State: ");
	uart_write_int(addr->state);
	uart_write_text(" counter: ");
	uart_write_int(addr->counter);
	uart_write_text(" priority: ");
	uart_write_int(addr->priority);
	uart_write_text(" preempt: ");
	uart_write_int(addr->preempt_count);
	uart_write_text("\n");
}

void _schedule() {
	disable_preempt();
	uart_write_text("Starting scheduler\n");
	int next,c;
	struct task_struct * p;
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
	uart_write_text("Switching to: ");
	uart_write_char('0' + next);
	uart_write_char('\n');
	switch_to(task[next]);
	enable_preempt();
}

void switch_to(struct task_struct* next) {
	if (current == next) return;
	print_proc(current);
	print_proc(next);
	struct task_struct * prev = current;
	current = next;
	uart_write_text("cpu_switch_to\n");
	cpu_switch_to(prev, next);
}

void schedule() {
	current->counter = 0;
	_schedule();
}

void sched_timer_tick() {
    current->counter--;
    // uart_write_text("in timer tick\n");
	if (current->counter > 0 || current->preempt_count > 0) {
		return;
	}
    current->counter = 0;
	enable_irq();
	_schedule();
	disable_irq();
}
