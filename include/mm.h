#include "io.h"

#define PAGE_SIZE               (1 << 12)

#define LOW_MEMORY              (1 << 15)   // TODO fix 
#define HIGH_MEMORY             PERIPHERAL_BASE

#define PAGES_TOTAL             ((HIGH_MEMORY-LOW_MEMORY)/PAGE_SIZE)

unsigned long get_free_page();
void free_page(unsigned long page);
