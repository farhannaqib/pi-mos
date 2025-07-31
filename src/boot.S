#include "sysregs.h"

.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Switch to EL1 
    ldr     x1, =SCTLR_VALUE_MMU_DISABLED
    msr     sctlr_el1, x1        

    ldr     x1, =HCR_VALUE
    msr     hcr_el2, x1

    ldr     x1, =SCR_VALUE
    msr     scr_el3, x1

    ldr     x1, =SPSR_VALUE
    msr     spsr_el3, x1

    adr     x1, el1_entry        
    msr     elr_el3, x1

    eret   
    
el1_entry:
    // Check processor ID is zero (executing on main core)
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbz     x0, main_core

    adr     x5, spin_cpu0
1:
    wfe
    ldr     x4, [x5, x0, lsl #3]
    cbz     x4, 1b
    
    ldr     x1, =_start
    mov     x2, 0x4000          // TODO fix this
    mul     x2, x2, x0
    add     x1, x1, x2 
    mov     sp, x1

    // kernel code relies on x0 = mpidr_el1[1:0]
    mov     x1, #0
    mov     x2, #0
    mov     x3, #0
    blr     x4

    b       hang

main_core:
    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
2:  cbz     w2, 3f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 2b               // Loop if non-zero

3:  // Set stack to start below our code
    ldr     x1, =_start
    mov     sp, x1

    // Jump to main()
    bl      main
    // In case it does return, halt the master core too
hang:
    b       hang


.ltorg

.org 0xd8
.globl spin_cpu0
spin_cpu0:
	.quad 0

.org 0xe0
.globl spin_cpu1
spin_cpu1:
	.quad 0

.org 0xe8
.globl spin_cpu2
spin_cpu2:
	.quad 0

.org 0xf0
.globl spin_cpu3
spin_cpu3:
    .quad 0
