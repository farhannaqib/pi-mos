#include "timer.h"
#include "io.h"
#include "sched.h"

const unsigned interval = 100000;
unsigned int curVal = 0;

void timer_init() {
    curVal = mmio_read(TIMER_CLO);
    curVal += interval;
    mmio_write(TIMER_C1, curVal);
}

void handle_timer_irq() {
    curVal += interval;
    mmio_write(TIMER_C1, curVal);
    mmio_write(TIMER_CS, TIMER_CS_C1);
    sched_timer_tick();
}
