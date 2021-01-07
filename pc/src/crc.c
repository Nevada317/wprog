#include "crc.h"

#define CRC_POLY  0x1021
#define CRC_START 0x0000
#define CRC_ShiftLeft 1

uint16_t CRC_Calculate(uint8_t* Start, uint16_t Length) {
	uint8_t* ptr = Start;
	uint16_t CrcBuff = CRC_START;
	for (uint16_t i = 0; i < Length; i++) {
		uint8_t new_value = *(ptr++);
		#if CRC_ShiftLeft
		CrcBuff ^= (new_value << 8);
		#else
		CrcBuff ^= (new_value);
		#endif
		for (uint8_t i = 0; i < 8; i++) {
			bool sol;
			#if CRC_ShiftLeft
			sol = (CrcBuff & 0x8000) != 0;
			CrcBuff <<= 1;
			#else
			sol = (CrcBuff & 0x0001) != 0;
			CrcBuff >>= 1;
			#endif
			if (sol)
				CrcBuff ^= CRC_POLY;
		}
	}
	return CrcBuff;
}
