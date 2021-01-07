#include <stdint.h>
typedef struct __attribute__ ((__packed__)) {
	uint8_t Status; // To be changed to Enum
	uint16_t UnsentLeft;
} endpoint_status;
