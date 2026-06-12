#include "io.h"
#include "irq.h"
#include "timer.h"
#include "utils.h"

void enable_interrupt_controller() {
    int reg_val = mmio_read(ENABLE_IRQS_1);
    mmio_write(ENABLE_IRQS_1, reg_val | SYSTEM_TIMER_1 | AUX_INT);
}

void disable_interrupt_controller() {
    int reg_val = mmio_read(ENABLE_IRQS_1);
    mmio_write(ENABLE_IRQS_1, reg_val & ~(SYSTEM_TIMER_1 | AUX_INT));
}

void handle_irq() {
    unsigned int irq = mmio_read(IRQ_PENDING_1);
  
    if (irq & SYSTEM_TIMER_1) handle_timer_irq();
    if (irq & AUX_INT) handle_uart_irq();

}
