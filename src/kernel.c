#include "io.h"
#include "framebuffer.h"

void main()
{
    uart_init();
    fb_init();

    uart_write_text("Hello from processor ");
    uart_write_char('0' + 1);
    uart_write_char('\n');
    draw_pixel(1, 1, 0x00FFFFFF);

    draw_string(5, 5, "fortnite", 0x00FFFFFF, 5);
    while (1);
}
