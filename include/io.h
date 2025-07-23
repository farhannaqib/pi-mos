#define PERIPHERAL_BASE 0x3F000000

/** writes 4-byte val to register reg */
void mmio_write(long reg, unsigned int val);
/** reads 4-byte val from register reg */
unsigned int mmio_read(long reg);

void delay(unsigned int count);

/** must be ran before using uart funcs */
void uart_init();
void uart_write_char(unsigned char ch);
void uart_write_text(char *buffer);
unsigned char uart_recv();
