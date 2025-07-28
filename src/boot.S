.section ".text.boot"  // Make sure the linker puts this at the start of the kernel image

.global _start  // Execution starts here

_start:
    // Check processor ID is zero (executing on main core)
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbnz    x0, 3f

    // Clean the BSS section
    ldr     x1, =__bss_start     // Start address
    ldr     w2, =__bss_size      // Size of the section
2:  cbz     w2, 3f               // Quit loop if zero
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 2b               // Loop if non-zero

3:  // Set stack to start below our code
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
