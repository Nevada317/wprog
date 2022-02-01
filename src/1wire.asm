MODULE_1WIRE_INIT:
	call UART_Init

	; RXD
	sbi PORTD, 0
	cbi DDRD, 0

	; TXD
	sbi PORTD, 1
	sbi DDRD, 1
ret

MODULE_1WIRE_DEINIT:
	cbi DDRD, 1 ; Release TXD
ret

MODULE_1WIRE_RXC:
; Called from ISR
; DATAI - byte received

ret

MODULE_1WIRE_TRYTOSEND:
; T flag - send data
; DATA - Byte to send

ret
