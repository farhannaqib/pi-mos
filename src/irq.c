#include "io.h"
#include "irq.h"
#include "timer.h"

void enable_interrupt_controller() {
    int reg_val = mmio_read(ENABLE_IRQS_1);
    mmio_write(ENABLE_IRQS_1, reg_val | SYSTEM_TIMER_1);
}

void disable_interrupt_controller() {
    int reg_val = mmio_read(ENABLE_IRQS_1);
    mmio_write(ENABLE_IRQS_1, reg_val & ~(SYSTEM_TIMER_1));
}

void handle_irq() {
    uart_write_text("IRQ handler called - ");
    unsigned int irq = mmio_read(IRQ_PENDING_1);
    switch (irq) {
        case (SYSTEM_TIMER_1):
            handle_timer_irq();
            break;
        default:
            break;
    }
}
