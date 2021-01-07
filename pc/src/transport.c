#include "transport.h"
#include "tty.h"
#include <stdlib.h>
#include <pthread.h>

#include <stdio.h>

#define STX   0xF0
#define ESC   0xF1
#define ESTX  0x70
#define EESC  0x71


static int port_handle = 0;
static void (*ParcelCallback)(transport_parcel*) = NULL;
static void (*EndpointStatusCallback)(uint8_t, endpoint_status*) = NULL;

volatile static bool ReaderStopReq = false;
pthread_t ReaderThread = 0;
void Reader_Start();
void Reader_Stop();
void* Reader(void* arg);
void Reader_NewByte(uint8_t byte);
static uint8_t RawRx_Buffer[260];
static volatile uint16_t RawRx_Counter = -1;
static uint16_t RawRx_Size = 0;


int TRANSPORT_OpenPort(const char *portname) {
	if (!portname)
		return -1;
	port_handle = tty_open(portname);
	if (port_handle <= 0)
		return -1;

	Reader_Start();

	return 0;
}

void TRANSPORT_Destroy() {
	Reader_Stop();

	if (port_handle) {
		tty_close(port_handle);
		port_handle = 0;
	}
}


void TRANSPORT_SendParcel(transport_parcel* parcel) {
	if (!parcel)
		return;
	uint8_t* RawBuffer = calloc(1, 10+256);
	// Start is not included into raw buffer
	uint8_t* ptr = RawBuffer;
	*(ptr++) = 0; // Length is not yet known
	*(ptr++) = 0; // Length is not yet known
	*(ptr++) = parcel->Instruction | (parcel->Bank2 ? 0x80 : 0x00);
	if (parcel->PayloadContainer) {
		*(ptr++) = parcel->PayloadContainer->PayloadLength;
		*(ptr++) = parcel->PayloadContainer->Offset & 0xFF;
		*(ptr++) = (parcel->PayloadContainer->Offset >>  8) & 0xFF;
		*(ptr++) = (parcel->PayloadContainer->Offset >> 16) & 0xFF;
		*(ptr++) = (parcel->PayloadContainer->Offset >> 24) & 0xFF;
		if (parcel->PayloadContainer->Payload) {
			for (uint16_t i = 0; i < parcel->PayloadContainer->PayloadLength; i++) {
				*(ptr++) = *(parcel->PayloadContainer->Payload + i);
			}
		} else {
			for (uint16_t i = 0; i < parcel->PayloadContainer->PayloadLength; i++) {
				*(ptr++) = 0x00;
			}
		}
	} else {
		*(ptr++) = 0;
	}
	uint8_t DataLength = ptr - RawBuffer;
	*(RawBuffer+0) = DataLength & 0x7F;
	*(RawBuffer+1) = (DataLength>>7) & 0x7F;
// 	uint16_t CRC = GetCrc(RawBuffer, DataLength);
// 	*(ptr++) = CRC & 0xFF;
// 	*(ptr++) = (CRC >> 8) & 0xFF;

	for (uint16_t i = 0; i < (DataLength+2); i++) {
		printf(" %02X", *(RawBuffer + i));
		if ((i & 15) == 15)
			printf("\n");
	}
	if ((DataLength+2) & 15)
		printf("\n");

	uint8_t* StuffedBuffer = calloc(1, 1+2*DataLength+2);
	uint8_t* sptr = StuffedBuffer;
	ptr = RawBuffer;
	*(sptr++) = STX;
	for (uint16_t i = 0; i < (DataLength+2); i++) {
		uint8_t byte = *(ptr++);
		if (byte == STX) {
			*(sptr++) = ESC;
			*(sptr++) = ESTX;
		} else if (byte == ESC) {
			*(sptr++) = ESC;
			*(sptr++) = EESC;
		} else {
			*(sptr++) = byte;
		}
	}

	free(RawBuffer);

	uint16_t StuffedLength = sptr - StuffedBuffer;

	for (uint16_t i = 0; i < StuffedLength; i++) {
		printf(" %02X", *(StuffedBuffer + i));
		if ((i & 15) == 15)
			printf("\n");
	}
	if (StuffedLength & 15)
		printf("\n");

	tty_write(port_handle, StuffedBuffer, StuffedLength);

	free(StuffedBuffer);

}

void TRANSPORT_SubscribeParcels(void (*callback)(transport_parcel*)) {
	ParcelCallback = callback;
}

void TRANSPORT_SubscribeEndpointStatus(void (*callback)(uint8_t, endpoint_status*)) {
	EndpointStatusCallback = callback;
}

transport_parcel* Parcel_Create(uint8_t PayloadSize) {
	transport_parcel* ptr = calloc(1, sizeof(transport_parcel*));
	if (PayloadSize) {
		ptr->PayloadContainer = calloc(1, sizeof(transport_payload*));
		ptr->PayloadContainer->PayloadLength = PayloadSize;
		ptr->PayloadContainer->Payload = calloc(1, PayloadSize);
	}
	return ptr;
}

void Parcel_Destroy(transport_parcel* parcel) {
	if (parcel) {
		if (parcel->PayloadContainer) {
			if (parcel->PayloadContainer->Payload) {
				free(parcel->PayloadContainer->Payload);
			}
			free(parcel->PayloadContainer);
		}
		free(parcel);
	}
}

// READER - Works in separate thread

void Reader_Start() {
	ReaderStopReq = false;
	RawRx_Counter = -1;
	pthread_create(&ReaderThread, NULL, &Reader, NULL);
}

void Reader_Stop() {
	if (!ReaderThread)
		return;
	ReaderStopReq = true;
	void * retval;
	pthread_join(ReaderThread, &retval);
}

void Reader_NewByte(uint8_t byte) {
	static bool escape = false;
	printf("Read: %02X\n", byte);
	if (byte == STX) {
		RawRx_Counter = 0;
		RawRx_Size = 4;
		escape = false;
		printf("> Start Detected\n");
	}

	if ((RawRx_Counter >= 0) && (RawRx_Counter < RawRx_Size)) {
		// Remove stuffing
		if (byte == ESC) {
			escape = true;
		} else {
			RawRx_Buffer[RawRx_Counter++] = byte | (escape ? 0x80 : 0);
			printf("Stored: RawRx_Buffer[%d] = %02X\n", RawRx_Counter - 1, RawRx_Buffer[RawRx_Counter - 1]);

			escape = false;
		}
	}


	if (RawRx_Counter >= RawRx_Size) {
		printf("> Time to check CRC\n");
		// Check CRC

		RawRx_Counter = -1;
	}
	// If RawRx_Counter reaches 3, then we have parcel size in bytes 1 and 2.
	// MSB is 2, LSB is 1. Both are 7-bit
	if (RawRx_Counter == 3) {
		RawRx_Size = (RawRx_Buffer[1] & 0x7F);
		RawRx_Size |= ((RawRx_Buffer[2] & 0x7F) << 7);
		if (RawRx_Size > 256)
			RawRx_Size = 256;
		RawRx_Size += 3; // Add STX and 2 bytes of CRC
	}
}

void* Reader(void* arg) {
	int rdlen;
	uint8_t RxArray[1024];
	printf("\nReader started\n");
	while (!ReaderStopReq && (rdlen = tty_read(port_handle, RxArray, sizeof(RxArray))) >= 0) {
		for (uint16_t i = 0; i < rdlen; i++) {
			Reader_NewByte(RxArray[i]);
		}
	}
	printf("\nReader terminates\n");
	return NULL;
}
