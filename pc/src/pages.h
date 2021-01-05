#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t index;
	bool Started;
	uint8_t OperationFlags;
	uint32_t Address;
	uint16_t PayloadSize;
	uint8_t* Payload;
	struct page_t* Next;
} page_t;

page_t* Page_Create(uint16_t PayloadSize);
page_t* Page_GetRoot();
void Page_DestroyAll();
