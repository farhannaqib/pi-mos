#include "io.h"
#include "framebuffer.h"

extern void _start();

void main(int core)
{
    if (core == 0) {
        uart_init();
        fb_init();
        
        mmio_write(0xE0, (unsigned long)_start);
        mmio_write(0xE8, (unsigned long)_start);
        mmio_write(0xF0, (unsigned long)_start);
        asm volatile ("sev");
    }
    
    draw_char('0' + core, 50, 50 + 20 * core, 0x00FFFFFF, 2);
    
    delay(core * 100000); // bc of writing text
    
    uart_write_text("Hello from processor ");
    uart_write_char(core + '0');
    uart_write_char('\n');
    
    while (1);
}
