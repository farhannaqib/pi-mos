.globl get_el
get_el:
	mrs x0, CurrentEL
	lsr x0, x0, #2
	ret

.globl interrupts_enabled
interrupts_enabled:
	mrs x0, daif
	ubfx x0, x0, #7, #1
	eor x0, x0, #1
	ret

.globl delay
delay:
	subs x0, x0, #1
	bne delay
	ret
