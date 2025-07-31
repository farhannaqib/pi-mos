#include "io.h"
#include "framebuffer.h"
#include "lib.h"
#include "utils.h"

extern void _start();

extern unsigned long spin_cpu0;
extern unsigned long spin_cpu1;
extern unsigned long spin_cpu2;
extern unsigned long spin_cpu3;

void wakeup_core(unsigned long *cpu, void *func) {
    // TODO
}

void main(int core)
{
    if (core == 0) {
        uart_init();
        uart_write_text("UART INITIALIZED");
        fb_init();
       
        // wake up cores
        mmio_write((long) &spin_cpu1, (unsigned long)&main);
        asm volatile ("sev");

        // run_shell();
        uart_write_char('0' + get_el());
        uart_write_char('\n');
    }

    if (core != 0) {

        draw_char('0' + core, 50 + (20 * core), 50, 0x00FFFFFF, 2);

        // draw_char('0' + core, 50, 50 + 20 * core, 0x00FFFFFF, 2);
        
        // delay(10); // bc of writing text

        uart_write_text("Processor ");
        uart_write_char(core + '0');
        uart_write_char('\n');

    }
}
