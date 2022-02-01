wdr
call MODULE_1WIRE_TRYTOSEND
brtc Loop_UART_NoSend

	rcall MODULE_1WIRE_EXCHANGE

	lds TEMP, UCSR0A
		sbrs TEMP, UDRE
			rjmp (PC-3)
	sts UDR0, DATA
	clt
Loop_UART_NoSend:
