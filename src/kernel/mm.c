#include "mm.h"

static unsigned short mem_map [ PAGES_TOTAL ] = {0,};

unsigned long get_free_page() {
    for (int i = 0; i < PAGES_TOTAL; i++) {
        if (mem_map[i] == 0) {
            mem_map[i] = 1;
            return LOW_MEMORY + i*PAGE_SIZE;
        }
    }
    return 0;
}

void free_page(unsigned long page) {
    mem_map[(page - LOW_MEMORY) / PAGE_SIZE] = 0;
}
