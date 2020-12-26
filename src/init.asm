clr NULL

ldc TEMP, SPL,  low(RAMEND)
ldc TEMP, SPH, high(RAMEND)

ldi XL,  low(RAMSTART)
ldi XH, high(RAMSTART)

; Memory clear loop
	st X+, NULL
	cpi XH, high(RAMEND+1)
brlo (PC-2)
