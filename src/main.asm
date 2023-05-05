.ifdef M164
.include "m164_custom.inc"
.endif

.ifdef M324
.include "m324_custom.inc"
.endif

.include "registers.asm"
.include "ram.asm"

.org addr_ISR_END
RESET:
	.include "macro.asm"
	.include "init.asm"

Loop:
	.include "loop.asm"
rjmp Loop

.include "mode.asm"
.include "uart.asm"

; That should be last directive in main file
.include "isr_table.asm"
