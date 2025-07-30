#include "io.h"
#include "utils.h"

void mmio_write(long reg, unsigned int val) { *(volatile unsigned int *)reg = val; }
unsigned int mmio_read(long reg) { return *(volatile unsigned int *)reg; }

enum {
    GPFSEL1         = PERIPHERAL_BASE + 0x200004,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUD           = PERIPHERAL_BASE + 0x200094,
    GPPUDCLK0       = PERIPHERAL_BASE + 0x200098
};

void gpio_init() {
    unsigned int selector = mmio_read(GPFSEL1);
    selector &= ~(7 << 15 | 7 << 12);       // clean pins
    selector |= (2 << 15 | 2 << 12);        // alt func 5
    mmio_write(GPFSEL1, selector);
    
    mmio_write(GPPUD, 0);
    delay(150);
    mmio_write(GPPUDCLK0, 1 << 14 | 1 << 15);
    delay(150);
    mmio_write(GPPUDCLK0, 0);
}

enum {
    AUX_BASE        = PERIPHERAL_BASE + 0x215000,
    AUX_IRQ         = PERIPHERAL_BASE,
    AUX_ENABLES     = AUX_BASE + 0x04,
    AUX_MU_IO_REG   = AUX_BASE + 0x40,
    AUX_MU_IER_REG  = AUX_BASE + 0x44,
    AUX_MU_IIR_REG  = AUX_BASE + 0x48,
    AUX_MU_LCR_REG  = AUX_BASE + 0x4C,
    AUX_MU_MCR_REG  = AUX_BASE + 0x50,
    AUX_MU_LSR_REG  = AUX_BASE + 0x54,
    AUX_MU_MSR_REG  = AUX_BASE + 0x58,
    AUX_MU_SCRATCH  = AUX_BASE + 0x5C,
    AUX_MU_CNTL_REG = AUX_BASE + 0x60,
    AUX_MU_STAT_REG = AUX_BASE + 0x64,
    AUX_MU_BAUD_REG = AUX_BASE + 0x68,
    
    AUX_UART_CLOCK  = 250000000,
    UART_MAX_QUEUE  = 16 * 1024
};

#define AUX_MU_BAUD(baud) ((AUX_UART_CLOCK/(baud*8))-1)

void uart_init() {
    mmio_write(AUX_ENABLES, 1);             // Enable mini UART
    mmio_write(AUX_MU_CNTL_REG, 0);         // Disable RX/TX
    mmio_write(AUX_MU_IER_REG, 0);          // Disable interrupts
    mmio_write(AUX_MU_IIR_REG, 6);          // Clear FIFOs, might not be necessary?
    mmio_write(AUX_MU_LCR_REG, 3);          // 8 bit mode
    mmio_write(AUX_MU_MCR_REG, 0);          // No autocontrol mode (no CTS/RTS)
    mmio_write(AUX_MU_BAUD_REG, AUX_MU_BAUD(115200));    // TODO

    gpio_init();

    mmio_write(AUX_MU_CNTL_REG, 3);     // Re-enable RX/TX
}

unsigned int uart_is_write_byte_ready() { return mmio_read(AUX_MU_LSR_REG) & 0x20; }

void uart_write_char(unsigned char ch) {
    while (!uart_is_write_byte_ready()) {};
    mmio_write(AUX_MU_IO_REG, ch);
}

void uart_write_text(char *buffer) {
    for (int i = 0; buffer[i] != '\0'; i++) {
        uart_write_char(buffer[i]);
    }
}

unsigned int uart_is_read_byte_ready() { return mmio_read(AUX_MU_LSR_REG) & 0x01; }

unsigned char uart_recv() {
    while (!uart_is_read_byte_ready()) {};
    return mmio_read(AUX_MU_IO_REG);
}
