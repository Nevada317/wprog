.ifdef M164
.include "m164_custom.inc"
.endif

.ifdef M324
.include "m324_custom.inc"
.endif

.include "registers.asm"
.include "ram.asm"

.cseg
.org addr_RESET
	jmp RESET

; ##############################
; ####   INTERRUPTS TABLE   ####
; ##############################

.org addr_INT0
	reti
	nop
.org addr_INT1
	reti
	nop
.org addr_INT2
	jmp Mode_ISR_FBUFF
.org addr_PCINT0
	reti
	nop
.org addr_PCINT1
	reti
	nop
.org addr_PCINT2
	reti
	nop
.org addr_PCINT3
	reti
	nop
.org addr_WDT
	reti
	nop
.org addr_TIMER2_COMPA
	reti
	nop
.org addr_TIMER2_COMPB
	reti
	nop
.org addr_TIMER2_OVF
	reti
	nop
.org addr_TIMER1_CAPT
	reti
	nop
.org addr_TIMER1_COMPA
	reti
	nop
.org addr_TIMER1_COMPB
	reti
	nop
.org addr_TIMER1_OVF
	reti
	nop
.org addr_TIMER0_COMPA
	reti
	nop
.org addr_TIMER0_COMPB
	reti
	nop
.org addr_TIMER0_OVF
	reti
	nop
.org addr_SPI_STC
	reti
	nop
.org addr_USART0_RX
	jmp UART_RXC_ISR
.org addr_USART0_UDRE
	reti
	nop
.org addr_USART0_TX
	reti
	nop
.org addr_ANALOG_COMP
	reti
	nop
.org addr_ADC
	reti
	nop
.org addr_EE_READY
	reti
	nop
.org addr_TWI
	reti
	nop
.org addr_SPM_READY
	reti
	nop
.org addr_USART1_RX
	reti
	nop
.org addr_USART1_UDRE
	reti
	nop
.org addr_USART1_TX
	reti
	nop

.org addr_ISR_END
; ####################################
; ####   INTERRUPTS TABLE ENDED   ####
; ####################################

RESET:
	.include "macro.asm"
	.include "init.asm"

Loop:
	.include "loop.asm"
rjmp Loop

.include "mode.asm"
.include "uart.asm"

