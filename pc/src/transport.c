#include "transport.h"
#include "tty.h"
#include <stdlib.h>

static int port_handle = 0;

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

}

void TRANSPORT_SubscribeParcels(void (*callback)(transport_parcel*)) {

}

void TRANSPORT_SubscribeEndpointStatus(void (*callback)(uint8_t, endpoint_status*)) {

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
