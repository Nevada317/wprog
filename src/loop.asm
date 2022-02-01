wdr
call MODULE_1WIRE_TRYTOSEND
brtc Loop_UART_NoSend

	rcall _MODULE_1W_SEND_R

	bst DATA, 0
	rcall _MODULE_1W_BIT_SEND

	lds TEMP, UCSR0A
		sbrs TEMP, UDRE
			rjmp (PC-3)
	sts UDR0, DATA
	clt
Loop_UART_NoSend:
