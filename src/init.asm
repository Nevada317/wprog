clr NULL
clr ONE
inc ONE

ldc TEMP, SPL,  low(RAMEND)
ldc TEMP, SPH, high(RAMEND)

ldi XL,  low(RAMSTART)
ldi XH, high(RAMSTART)

; Memory clear loop
	st X+, NULL
	cpi XH, high(RAMEND+1)
brlo (PC-2)

ldc TEMP, PORTA, 0xFF
ldc TEMP, PORTB, 0x7F
ldc TEMP, PORTC, 0xFF
ldc TEMP, PORTD, 0xCC
ldc TEMP, DDRA,  0x00
ldc TEMP, DDRB,  0xB0
ldc TEMP, DDRC,  0x0C
ldc TEMP, DDRD,  0xC0

call Mode_Init
call UART_Init

sei
