#include <stdint.h>
#include <stdbool.h>

#define PAGES_CHIPERASE (1<<7)

typedef struct {
	uint8_t index;
	bool Started;
	bool DataFlag;
	uint8_t OperationFlags;
	uint32_t Address;
	uint16_t PayloadSize;
	uint8_t* Payload;
	struct page_t* Next;
} page_t;

page_t* Page_Create(uint16_t PayloadSize);
page_t* Page_Create_SignatureRead();
page_t* Page_Create_ChipErase();
page_t* Page_FindPageByAddress(uint32_t address, uint16_t pagesize);

page_t* Page_GetRoot();
void Page_DestroyAll();
