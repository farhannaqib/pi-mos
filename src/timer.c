#include "timer.h"
#include "io.h"
#include "sched.h"
#include "irq.h"

const unsigned interval = 50000;
unsigned int curVal = 0;

void timer_init() {
    curVal = mmio_read(TIMER_CLO);
    curVal += interval;
    mmio_write(TIMER_C1, curVal);
}

void handle_timer_irq() {
    mmio_write(TIMER_CS, TIMER_CS_C1);
    unsigned now = mmio_read(TIMER_CLO);
    do {    // catch-up loop
        curVal += interval;
    } while (curVal < now);
    mmio_write(TIMER_C1, curVal);
    sched_timer_tick();
}
