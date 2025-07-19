#include "io.h"
#include "framebuffer.h"

extern void _start();

void main(int core)
{
    if (core == 0) {
        uart_init();
        fb_init();

        // *(volatile unsigned long *)0xE0 = (unsigned long)&main;
        mmio_write(0xE0, (unsigned long)_start);
        asm volatile ("sev");
    }

    delay(core * 100000);
    
    uart_write_text("Hello from processor ");
    uart_write_char(core + '0');
    uart_write_char('\n');
    

    if (core == 0) {
        draw_pixel(1, 1, 0x00FFFFFF);
        draw_string(5, 5, "fortnite", 0x00FFFFFF, 5);
    }
    while (1);
}
