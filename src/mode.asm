Mode_Update:
	push TEMP

	lds TEMP, Mode_State
		cpi TEMP, Mode_State_UART
			brne (PC+2)
				rcall Mode_Set_UART
	lds TEMP, Mode_State
		cpi TEMP, Mode_State_HIO
			brne (PC+2)
				rcall Mode_Set_HIO
	lds TEMP, Mode_State
		cpi TEMP, Mode_State_MPRG
			brne (PC+2)
				rcall Mode_Set_MPRG

	pop TEMP
ret

Mode_Set_UART:
	sts DDRA, NULL ; Release PAR port
	cbi DDRD, 0 ; Release PAR
	cbi DDRD, 1
	cbi DDRD, 2
	cbi DDRD, 3
	cbi PORTC, 2 ; Select UART
	sbi PORTC, 3 ; Deselect MCU_BUFF
ret

Mode_Set_HIO:
	sbi PORTC, 2 ; Deselect UART
	sbi PORTC, 3 ; Deselect MCU_BUFF
; 	sbi PORTD, 7
ret

Mode_Set_MPRG:
	sbi PORTC, 2 ; Deselect UART
; 	sbi PORTD, 7
ret

Mode_ISR_FBUFF:
	in rSREG, SREG
		push TEMP
			in TEMP, PINB
			andi TEMP, (1<<2)
				brne _Mode_ISR_FBUFF_exit
			_Mode_ISR_FBUFF_enter:
				ldi TEMP, Mode_State_HIO
				rjmp _Mode_ISR_FBUFF_common
			_Mode_ISR_FBUFF_exit:
				ldi TEMP, Mode_State_UART
				; rjmp _Mode_ISR_FBUFF_common
			_Mode_ISR_FBUFF_common:
			sts Mode_State, TEMP
		pop TEMP
		rcall Mode_Update
	out SREG, rSREG
reti

Mode_Init:
	; Any edge shoult trigger interrupt
	lds TEMP, EICRA
	cbr TEMP, (1<<5); ISC21
	sbr TEMP, (1<<4); ISC20
	sts EICRA, TEMP

	; Enable interrupt
	sbi EIMSK, 2 ; INT2

	rcall Mode_Update
ret
