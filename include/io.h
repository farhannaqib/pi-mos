#define PERIPHERAL_BASE 0x3F000000

/** writes 4-byte val to register reg */
void mmio_write(long reg, unsigned int val);
/** reads 4-byte val from register reg */
unsigned int mmio_read(long reg);

/** must be ran before using uart funcs */
void uart_init();
void uart_write_char(unsigned char ch);
void uart_write_text(char *buffer);
/** non-blocking, reads from static buffer populated by irq handler */
int uart_read();
void handle_uart_irq();
