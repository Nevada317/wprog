UART_Init:
	ldc TMPI, UCSR0A, 0b00000000
	ldc TMPI, UCSR0B, 0b10011000
	ldc TMPI, UCSR0C, 0b00001110
	ldc TMPI, UBRR0L, 16 ; BAUD = 57600
; 	ldc TMPI, UBRR0L, 12 ; BAUD = 76800
ret

UART_RXC_ISR:
	in rSREG, SREG

	lds DATAI, UDR0
	cpi DATAI, 0x02
		brne _UART_RXC_ISR_nostart
	lds TMPI, Mode_State
	cpi TMPI, Mode_State_UART
		brne _UART_RXC_ISR_nostart

		cbi PORTD, 7
; 		ldc TMPI, Mode_State, Mode_State_MPRG
		rcall Mode_Update

	_UART_RXC_ISR_nostart:
	sts UDR0, DATAI

	out SREG, rSREG
reti
