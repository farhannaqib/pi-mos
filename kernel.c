#include "io.h"
#include "framebuffer.h"

void main()
{
    uart_init();
    fb_init();
    uart_writeText("amogus");
    draw_pixel(1, 1, 0x00FFFFFF);
    draw_string(5, 5, "amogus", 0x00FFFFFF);
    while (1);
}
