#include "io.h"
#include "framebuffer.h"
#include "lib.h"
#include "utils.h"
#include "irq.h"

extern void _start();

extern unsigned long spin_cpu0;
extern unsigned long spin_cpu1;
extern unsigned long spin_cpu2;
extern unsigned long spin_cpu3;

void wakeup_core(unsigned long *addr, void (*func)()) {
    mmio_write((long) addr, (unsigned long)func);
    asm volatile ("sev");
}

void main(int core)
{
    if (core == 0) {
        irq_vector_init();

        uart_init();
        uart_write_text("UART INITIALIZED");
        fb_init();
       
        // wake up cores
        wakeup_core(&spin_cpu1, &main);
        wakeup_core(&spin_cpu2, &main);
        wakeup_core(&spin_cpu3, &main);

        // run_shell();
        uart_write_char('0' + get_el());
        uart_write_char('\n');
    }
    
    //delay(core * 100000000 + 1);

    draw_char('0' + get_el(), 50 + (20 * core), 50, 0x00FFFFFF, 2);
    // draw_char('0' + core, 50, 50 + 20 * core, 0x00FFFFFF, 2);
    

    delay(1000000 * (core + 1));
    uart_write_text("Processor ");
    uart_write_char(core + '0');
    uart_write_text(" - Exception Level: ");
    uart_write_char(get_el() + '0');
    uart_write_char('\n');

}
