#include "transport.h"
#include "tty.h"
#include <stdlib.h>

#include <stdio.h>


static int port_handle = 0;
static void (*ParcelCallback)(transport_parcel*) = NULL;
static void (*EndpointStatusCallback)(uint8_t, endpoint_status*) = NULL;


int TRANSPORT_OpenPort(const char *portname) {
	if (!portname)
		return -1;
	port_handle = tty_open(portname);
	if (port_handle <= 0)
		return -1;

	// TODO: Here we should prepare our second thread
	return 0;
}

void TRANSPORT_Destroy() {
	// TODO: Here we should stop and join our second thread
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

	for (uint16_t i = 0; i < DataLength; i++) {
		printf(" %02X", *(RawBuffer + i));
		if ((i & 15) == 15)
			printf("\n");
	}
	if (DataLength & 15)
		printf("\n");
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
