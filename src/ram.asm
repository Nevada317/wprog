.dseg
.org RAMSTART

RAM_DataBuffer1: .BYTE 256
RAM_DataBuffer2: .BYTE 256

Mode_State: .BYTE 1
.equ Mode_State_UART = 0
.equ Mode_State_HIO  = 1
.equ Mode_State_MPRG = 2
