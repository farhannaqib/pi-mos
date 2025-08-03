#include "io.h"

#define TIMER_CS        (PERIPHERAL_BASE + 0x00003000)
#define TIMER_CLO       (PERIPHERAL_BASE + 0x00003004)
#define TIMER_CHI       (PERIPHERAL_BASE + 0x00003008)
#define TIMER_C0        (PERIPHERAL_BASE + 0x0000300C)
#define TIMER_C1        (PERIPHERAL_BASE + 0x00003010)
#define TIMER_C2        (PERIPHERAL_BASE + 0x00003014)
#define TIMER_C3        (PERIPHERAL_BASE + 0x00003018)

#define TIMER_CS_C1     (1 << 1)
#define TIMER_CS_C3     (1 << 3)

void timer_init();
void handle_timer_irq();
