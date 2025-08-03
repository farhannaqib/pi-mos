#include "io.h"

#define IRQ_BASIC_PENDING   (PERIPHERAL_BASE + 0x0000B200)
#define IRQ_PENDING_1       (PERIPHERAL_BASE + 0x0000B204)
#define IRQ_PENDING_2       (PERIPHERAL_BASE + 0x0000B208)
#define FIQ_CONTROL         (PERIPHERAL_BASE + 0x0000B20C)
#define ENABLE_IRQS_1       (PERIPHERAL_BASE + 0x0000B210)
#define ENABLE_IRQS_2       (PERIPHERAL_BASE + 0x0000B214)
#define ENABLE_BASIC_IRQS   (PERIPHERAL_BASE + 0x0000B218)
#define DISABLE_IRQS_1      (PERIPHERAL_BASE + 0x0000B21C)
#define DISABLE_IRQS_2      (PERIPHERAL_BASE + 0x0000B220)
#define DISABLE_BASIC_IRQS  (PERIPHERAL_BASE + 0x0000B224)

#define SYSTEM_TIMER_1      (1 << 1)
#define SYSTEM_TIMER_3      (1 << 3)

void irq_vector_init();
void enable_irq();
void disable_irq();

void enable_interrupt_controller();
void disable_interrupt_controller();
