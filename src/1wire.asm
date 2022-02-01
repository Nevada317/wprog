MODULE_1WIRE_INIT:
	call UART_Init

	; RXD
	sbi PORTD, 0
	cbi DDRD, 0

	; TXD
	sbi PORTD, 1
	sbi DDRD, 1

	sbi PORTB, 2 ; Disable buffer
	sbi DDRB, 2

	sbi PORTC, 2 ; Deselect UART
	cbi PORTC, 3 ; Select MCU_BUFF

ret

MODULE_1WIRE_DEINIT:
	cbi DDRD, 1 ; Release TXD
ret

MODULE_1WIRE_RXC:
; Called from ISR
; DATAI - byte received
	ldi TMPI, 1
	eor DATAI, TMPI
	sts RAM_1W_ByteValue, DATAI
	sts RAM_1W_RxcFlag, ONE
ret

MODULE_1WIRE_TRYTOSEND:
; T flag - send data
; DATA - Byte to send
	clt
	lds TEMP, RAM_1W_RxcFlag
	tst TEMP
		brne (PC+2)
			ret
	sts RAM_1W_RxcFlag, NULL
	lds DATA, RAM_1W_ByteValue
	set
ret
