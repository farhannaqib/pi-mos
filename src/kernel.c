#include "io.h"
#include "framebuffer.h"

extern void _start();

void main(int core)
{
    if (core == 0) {
        uart_init();
        fb_init();
       
        // wake up cores
        // ideally this is done after core 0 initializes everything
        // originally this was in src/boot.S, but bc the event register
        // might not be cleared on bootup, other cores will skip the spin 
        // loop and immediately start executing main, as core 0 is still 
        // initializing... rlly annoying to debug
        // mmio_write(0xE0, (unsigned long)_start);
        // mmio_write(0xE8, (unsigned long)_start);
        // mmio_write(0xF0, (unsigned long)_start);
        asm volatile ("sev");
    }
    
    draw_char('0' + core, 50, 50 + 20 * core, 0x00FFFFFF, 2);
    
    // delay(core * 1000000); // bc of writing text
    
    for (int i = 0; i < 5; i++) {
        // delay(500000000);
        // uart_write_text("Processor ");
        // uart_write_char((i/4) + core + '0');
        // uart_write_char('\n');
        uart_write_char('a' + i);
        unsigned char ch = uart_recv();
        draw_char(ch, 50, 50 + 20 * (i + 1), 0x00FFFFFF, 2);
    }
    
    while (1);
}
