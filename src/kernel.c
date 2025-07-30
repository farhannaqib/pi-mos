#include "io.h"
#include "framebuffer.h"
#include "lib.h"
#include "utils.h"

extern void _start();

// must be done AFTER init funcs are called
void wakeup_cores() {
    // originally this was in boot.S, but bc the event register
    // might not be cleared on bootup, other cores will skip the spin 
    // loop and immediately start executing main
    
    // mmio_write(0xE0, (unsigned long)_start);
    // mmio_write(0xE8, (unsigned long)_start);
    // mmio_write(0xF0, (unsigned long)_start);
    // asm volatile ("sev");
}

void main(int core)
{
    if (core == 0) {
        uart_init();
        fb_init();
       
        // wake up cores
        wakeup_cores();

        run_shell();
    }
    
    char buf[2] = {0};
    itoa(buf, get_el());
    uart_write_text(buf);

    // draw_char('0' + core, 50, 50 + 20 * core, 0x00FFFFFF, 2);
    
    // delay(core * 1000000); // bc of writing text
    
    // uart_write_text("Processor ");
    // uart_write_char((i/4) + core + '0');
    // uart_write_char('\n');
}
