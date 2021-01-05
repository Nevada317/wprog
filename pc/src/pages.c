#include "pages.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


static page_t* Pages_Root;

static uint8_t Page_GetNextIndex() {
	static uint8_t curIndex = 0;
	curIndex++;
	if (!curIndex)
		curIndex = 1;
	return curIndex;
}

page_t* Page_Create_SignatureRead() {
	page_t* ptr = Page_Create(0);
// 	ptr->OperationFlags = 0;
	return ptr;
}

page_t* Page_Create_ChipErase() {
	page_t* ptr = Page_Create(0);
	ptr->OperationFlags = PAGES_CHIPERASE;
	return ptr;
}

page_t* Page_Create(uint16_t PayloadSize) {
	page_t* ptr;
	if (Pages_Root == NULL) {
		Pages_Root = calloc(1, sizeof(page_t));
		ptr = Pages_Root;
	} else {
		ptr = Pages_Root;
		while ((ptr->Next) != NULL)
			ptr = (page_t*) ptr->Next;

		ptr->Next = calloc(1, sizeof(page_t));
		ptr = (page_t*) ptr->Next;
	}
	ptr->index = Page_GetNextIndex();
	ptr->Started = false;
	ptr->OperationFlags = 0;
	ptr->Address = 0;
	ptr->PayloadSize = PayloadSize;
	if (ptr->PayloadSize > 0) {
		if (ptr->PayloadSize > 256)
			ptr->PayloadSize = 256;
		memset(ptr->Payload = malloc(ptr->PayloadSize), 0xFF, ptr->PayloadSize);
	} else {
		ptr->Payload = NULL;
	}
	return ptr;
}

page_t* Page_GetRoot() {
	return Pages_Root;
}

static void Page_DestroyFirst() {
	page_t* ptr;
	ptr = (page_t*) Pages_Root->Next;

	if (Pages_Root->Payload)
		free(Pages_Root->Payload);

	free(Pages_Root);
	Pages_Root = ptr;
}

void Page_DestroyAll() {
	while (Pages_Root)
		Page_DestroyFirst();
}
