#include "io.h"
#include "framebuffer.h"

void main()
{
    uart_init();
    fb_init();
    uart_writeText("amogus");
    draw_pixel(1, 1, 0x00FFFFFF);
    draw_string(5, 5, "fortnite", 0x00FFFFFF, 5);
    while (1);
}
