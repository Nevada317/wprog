wdr

call MODULE_1WIRE_TRYTOSEND
brtc Loop_UART_NoSend
	sbis UCSR0A, UDRE
		rjmp (PC-1)
	out UDR0, DATA
	clt
Loop_UART_NoSend:
