#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ihex.h"

uint32_t DATA_offset  = 0;

void ihex_analyze_line(char* start, char* end);
uint8_t HexCharToDec(char* start);
uint8_t Hex8ToDec(char** start);
uint16_t Hex16ToDec(char** start);

void (*NewByteCallback)(uint8_t, uint32_t);

int ihex_read(char* filename, void (*NewByteFunction)(uint8_t, uint32_t)) {
	NewByteCallback = NewByteFunction;
	FILE* fd = fopen(filename, "r");
	if (NULL == fd) {
		printf("ihex: Unable to open file\n");
		return -1;
	}
	fseek(fd, 0L, SEEK_END);
	long int filesize = ftell(fd);
	fseek(fd, 0L, SEEK_SET);
	char* textbuffer = malloc(filesize+4);
// 	printf("ihex: File opened. Size is %ld\n", filesize);

	DATA_offset = 0;

	size_t fr = fread(textbuffer, 1, filesize, fd);
	*(textbuffer+fr++)=13;
	*(textbuffer+fr++)=10;
	*(textbuffer+fr)=0;
	char* textbuffer_linestart = textbuffer;
	for (char* ptr = textbuffer; ptr<textbuffer+fr; ptr++) {
		if ((*(ptr) == 10) || (*(ptr) == 13)) {
			if ((ptr-textbuffer_linestart) > 10)
				ihex_analyze_line(textbuffer_linestart, ptr-1);
			textbuffer_linestart = ptr+1;
		}
	}
	fclose(fd);
	return 0;
}

void ihex_analyze_line(char* start, char* end) {
	if (*(start++) != ':') {
		printf("ihex: Wrong start symbol! Ignored.\n");
		return;
	}
	uint8_t numel = Hex8ToDec(&start);
	uint16_t addr = Hex16ToDec(&start);
	uint8_t type = Hex8ToDec(&start);

	if ((type != 0) && (type != 1) && (type != 2)) {
		printf("ihex: Record type %02x not supported! Ignored.\n", type);
	}
	if (type == 0) {
		uint32_t address = addr + DATA_offset;
// 		printf("ihex: Record type %02x. Addr = %06x, dataN = %d\n", type, address, numel);

		for (uint8_t num=0; num<numel; num++) {
			uint8_t databyte = Hex8ToDec(&start); // That byte should be stored
			if (NewByteCallback)
				(*NewByteCallback)(databyte, address);
			address++; // After each storage address is incremented
		}
		Hex8ToDec(&start); // Dummy read for checksum
	}
	if (type == 2) {
		if (numel == 2) {
			DATA_offset = Hex16ToDec(&start) * 16;
		} else {
			printf("ihex: Record type %02x with length %02x found. Ignored.\n", type, numel);
		}
		Hex8ToDec(&start); // Dummy read for checksum
	}

	return;
}




uint8_t HexCharToDec(char* start) {
	const char* alphabet = "0123456789ABCDEF";
	char* ptr;
	ptr = strchr(alphabet, (int)(*start));
	if (NULL == ptr) {
		printf("Bad character: \"%c\"!\n", *start);
		return 0;
	}
	return (uint8_t)(ptr - alphabet);
}

uint8_t Hex8ToDec(char** start) {
	uint8_t temp;
	temp = HexCharToDec((*start)++);
	temp <<= 4;
	temp += HexCharToDec((*start)++);

	return temp;
}

uint16_t Hex16ToDec(char** start) {
	uint16_t temp;
	temp = HexCharToDec((*start)++);
	temp <<= 4;
	temp += HexCharToDec((*start)++);
	temp <<= 4;
	temp += HexCharToDec((*start)++);
	temp <<= 4;
	temp += HexCharToDec((*start)++);
	return temp;
}
