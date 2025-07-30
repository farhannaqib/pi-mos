#include "sysregs.h"

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core)
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbnz    x0, 3f

    // Switch to EL1 
    ldr     x0, =SCTLR_VALUE_MMU_DISABLED
    msr     sctlr_el1, x0        

    ldr     x0, =HCR_VALUE
    msr     hcr_el2, x0

    ldr     x0, =SPSR_VALUE
    msr     spsr_el2, x0

    adr     x0, el1_entry        
    msr     elr_el2, x0

    eret   

el1_entry:
    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
2:  cbz     w2, 3f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 2b               // Loop if non-zero

3:  // Set stack to start below our code
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    ldr     x1, =_start
    mov     x2, 0x4000          // TODO fix this
    mul     x2, x2, x0
    add     x1, x1, x2 
    mov     sp, x1

    // Jump to main()
    bl      main
    // In case it does return, halt the master core too
hang:
    b       hang
