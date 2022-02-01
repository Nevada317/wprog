.equ CyclesPerMs = F_OSC / 1000 / 5

MODULE_1WIRE_INIT:
	call UART_Init

	; RXD
	sbi PORTD, 0
	cbi DDRD, 0

	; TXD
	sbi PORTD, 1
	sbi DDRD, 1

	sbi PORTB, 2 ; Disable buffer (from 2232)
	sbi DDRB, 2

	cbi PORTC, 3 ; Enable buffer (from MCU)
	sbi DDRC, 3

	sbi PORTC, 2 ; Deselect UART
	cbi PORTC, 3 ; Select MCU_BUFF

	sbi DDRB, 5 ; MOSI - output
	cbi PORTB, 5

	cbi DDRB, 6 ; MISO - input
	sbi PORTB, 6

	sts RAM_1W_RX_WP, NULL
	sts RAM_1W_RX_RP, NULL

	sts RAM_1W_TX_WP, NULL
	sts RAM_1W_TX_RP, NULL
ret

MODULE_1WIRE_DEINIT:
	cbi DDRD, 1 ; Release TXD
	sbi PORTC, 3 ; Disable buffer (from MCU)
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


; ##########################################################

_MODULE_1W_PULL:
	sbi PORTB, 5
ret

_MODULE_1W_RELEASE:
	cbi PORTB, 5
ret

_MODULE_1W_SAMPLE:
	in TEMP, PINB
	bst TEMP, 6
ret



_MODULE_1W_SEND_R:
; T - return detection
	push DATA
	push ADDR
	push TEMP
	rcall _MODULE_1W_PULL
	rcall _MODULE_1W_DELAY_600us
	rcall _MODULE_1W_RELEASE
	rcall _MODULE_1W_DELAY_10us

	ldi DATA, 1
	ldi ADDR, 200 ; x3 us
	_MODULE_1W_SEND_R_loop:
		rcall _MODULE_1W_DELAY_3us
		clr TEMP
		bld TEMP, 0
		or DATA, TEMP
		dec ADDR
	brne _MODULE_1W_SEND_R_loop
	com DATA
	bst DATA, 0
	pop TEMP
	pop ADDR
	pop DATA
ret

_MODULE_1W_BIT_SEND:
; T - input
	rcall _MODULE_1W_PULL
	rcall _MODULE_1W_DELAY_10us
	brtc (PC+2)
		rcall _MODULE_1W_RELEASE
	rcall _MODULE_1W_DELAY_80us
	rcall _MODULE_1W_RELEASE
	rcall _MODULE_1W_DELAY_10us
ret

_MODULE_1W_BIT_READ:
; T - output
	rcall _MODULE_1W_PULL
	rcall _MODULE_1W_DELAY_1us
	rcall _MODULE_1W_RELEASE
	rcall _MODULE_1W_DELAY_10us
	rcall _MODULE_1W_SAMPLE
	rcall _MODULE_1W_DELAY_30us
ret




_MODULE_1W_DELAY_1us:
	ldi ZL,  low(3 * CyclesPerMs / 2000)  ; 1.5 us
	ldi ZH, high(3 * CyclesPerMs / 2000)  ; 1.5 us
rjmp _1w_delay_loop

_MODULE_1W_DELAY_3us:
	ldi ZL,  low(3 * CyclesPerMs / 1000)
	ldi ZH, high(3 * CyclesPerMs / 1000)
rjmp _1w_delay_loop

_MODULE_1W_DELAY_10us:
	ldi ZL,  low(10 * CyclesPerMs / 1000)
	ldi ZH, high(10 * CyclesPerMs / 1000)
rjmp _1w_delay_loop

_MODULE_1W_DELAY_15us:
	ldi ZL,  low(15 * CyclesPerMs / 1000)
	ldi ZH, high(15 * CyclesPerMs / 1000)
rjmp _1w_delay_loop

_MODULE_1W_DELAY_30us:
	ldi ZL,  low(30 * CyclesPerMs / 1000)
	ldi ZH, high(30 * CyclesPerMs / 1000)
rjmp _1w_delay_loop

_MODULE_1W_DELAY_80us:
	ldi ZL,  low(80 * CyclesPerMs / 1000)
	ldi ZH, high(80 * CyclesPerMs / 1000)
rjmp _1w_delay_loop

_MODULE_1W_DELAY_600us: ; Reset pulse
	ldi ZL,  low(600 * CyclesPerMs / 1000)
	ldi ZH, high(600 * CyclesPerMs / 1000)
rjmp _1w_delay_loop


_1w_delay_loop:
	wdr
	sub ZL, ONE
	sbc ZH, NULL
brcc _1w_delay_loop
ret

