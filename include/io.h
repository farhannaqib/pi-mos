#define PERIPHERAL_BASE 0x3F000000

void mmio_write(long reg, unsigned int val);
unsigned int mmio_read(long reg);

void delay(unsigned int count);

void uart_init();
void uart_write_char(unsigned char ch);
void uart_write_text(char *buffer);

