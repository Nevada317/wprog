//
//   Possible instructions (commands) on transport level
//
// NOTE: In instruction, bit 7 selects bank. Instructions 00-0F have no bank
// Just status request. Does not affect anything
#define Command_Poll               0x00
// Write access description struct (SPI speed etc). Offset should be equal 0
#define Command_SetAccessDesc      0x01
// Write device description struct (target signature, pagination etc). Offset should be equal 0
#define Command_SetDeviceDesc      0x02
// Return to simple UART bridge mode
#define Command_ReleaseUART        0x0E
// Hard-reset USB interface
#define Command_ResetUSB           0x0F
// Write header,for bank, effectively claiming it
#define Command_SetHeader          0x11
// Add data to pre-claimed bank. Offset should match it's value in Status structure
#define Command_PutDataBlock       0x12
// Get data from bank. Useful for all read instructions, including signature read
#define Command_GetDataBlock       0x13
// Remove header from bank, effectively releasing it
#define Command_ReleaseBank        0x1F
// This constant should be ORed to instruction to select second bank
#define _Command_Bank2             0x80


//
//   BufferStatus possible values
//
// Master should send header to claim buffer
#define BufferStatus_Empty         0x00
// Buffer claimed. Waiting for data. PC should send multiple packets to fill buffer
#define BufferStatus_Filling       0x01
// Data received or not needed. MCU waits for completion of another task
#define BufferStatus_Queued        0x02
// MCU sends data to target. PC should wait.
#define BufferStatus_Writing       0x03
// Write delay/polling while target writes page. PC waits and polls MCU
#define BufferStatus_Waiting       0x04
// PC reads data from MCU to verify/read
#define BufferStatus_Reading       0x05
// Operation finished. Read needed. PC may send multiple read commands. ACK to exit
#define BufferStatus_DataReady     0x06
// Operation finished (OK). ACK to exit
#define BufferStatus_FinishOK      0x7E
// Operation finished with ERROR. ACK to exit
#define BufferStatus_FinishERR     0x7F
// Programmer not initialized. PC should issue Command_SetAccessDesc first
#define BufferStatus_NoAccessDesc  0x81
// Programmer not initialized. PC should issue Command_SetDeviceDesc first
#define BufferStatus_NoDeviceDesc  0x82


//
//   Operations, that can be defined in bank header
//
// Enter programming
#define Operation_EnterProg        0x01
// Chip erase
#define Operation_ChipErase        0x02
// Read params (Signature, fuses)
#define Operation_ReadParams       0x03
// Write flash
#define Operation_WriteFlash       0x04
// Write EEPROM
#define Operation_WriteEEPROM      0x05
// Write fuse/lock. Offset selects fuses
#define Operation_WriteFuses       0x06
// Exit programming
#define Operation_ExitProgramming  0x0E
// Reset target
#define Operation_ResetTarget      0x0F
