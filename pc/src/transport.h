#include <stdint.h>
#include <stdbool.h>
#include "endpoint.h"

typedef struct __attribute__ ((__packed__)) {
	uint8_t PayloadLength;
	uint32_t Offset;
	uint8_t* Payload;
} transport_payload;

typedef struct __attribute__ ((__packed__)) {
	uint8_t FullLength; // Will be calculated by sender
	bool Bank2;
	uint8_t Instruction;
	transport_payload* PayloadContainer;
} transport_parcel;


int TRANSPORT_OpenPort(const char *portname);
void TRANSPORT_SendParcel(transport_parcel* parcel);
void TRANSPORT_SubscribeParcels(void (*callback)(transport_parcel*));
void TRANSPORT_SubscribeEndpointStatus(void (*callback)(uint8_t, endpoint_status*));
void TRANSPORT_Destroy();

transport_parcel* Parcel_Create(uint8_t PayloadSize);
void Parcel_Destroy(transport_parcel* parcel);
