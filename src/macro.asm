.macro ldc
; Load constant to IO register, usage:
;	ldc R16, DDRC, 0b01000101

	ldi @0, @2
	#if (@1>0x3F)
		sts @1, @0
	#else
		out @1, @0
	#endif
.endmacro
