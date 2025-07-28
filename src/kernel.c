#include "io.h"
#include "framebuffer.h"
#include "lib.h"
#include "utils.h"

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
        // asm volatile ("sev");
    }
    
    char buf[2] = {0};
    itoa(buf, get_el());
    uart_write_text(buf);

    // draw_char('0' + core, 50, 50 + 20 * core, 0x00FFFFFF, 2);
    
    // delay(core * 1000000); // bc of writing text
    
    while (1) {
        // delay(500000000);
        // uart_write_text("Processor ");
        // uart_write_char((i/4) + core + '0');
        // uart_write_char('\n');
        run_shell();
    }
}
