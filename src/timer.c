#include "timer.h"
#include "io.h"

const unsigned interval = 2000000;
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
    uart_write_text("Timer interrupt recieved\n");
}
