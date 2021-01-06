#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "task.h"
#include "pages.h"
#include "ihex.h"

static uint16_t PageSize = 16;

void NewByte(uint8_t data, uint32_t address) {
// 	printf("  D.0x%06x = 0x%02x ", address, data);
	page_t* ptr = Page_FindPageByAddress(address, PageSize);
	if (ptr == NULL) {
		ptr = Page_Create(PageSize);
		ptr->DataFlag = true;
		ptr->Address = address & ~(PageSize - 1);
	}
// 	printf(" %d\n", ptr->index);

	*(ptr->Payload + (address & (ptr->PayloadSize - 1))) = data;
}

int main(int argc, char *argv[]) {
	printf("Program started\n");

	char* strPort = NULL;
	bool Verify = true;

	task_t* task;

	printf("Parsing input parameters...\n");
	// https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
	int opt;
	while((opt = getopt(argc, argv, ":p:f:e:scVv")) != -1) {
		switch (opt) {
			case 'p':
				if (strPort) {
					free(strPort);
					strPort = NULL;
				}
				strPort = malloc(strlen(optarg)+1);
				strcpy(strPort, optarg);
				printf("  Port is \"%s\"\n", strPort);
				break;
			case 'V':
				Verify = false;
				break;
			case 'v':
				Verify = true;
				break;
			case 's':
				Page_Create_SignatureRead();
				break;
			case 'c':
				Page_Create_ChipErase();
				break;
			case 'f':
			case 'e':
				task = TASK_Create();
				task->Verify = Verify;
				task->EEP = (opt == 'e');
				strcpy(task->Filename = malloc(strlen(optarg)+1), optarg);
				printf("  Task created: %c:\"%s\", Verify=%s\n", opt, task->Filename, Verify ? "true" : "false");
				break;
			case ':':
				printf("  Option %c needs a value\n", opt);
				break;
			case '?':
				printf("  Unknown option: %c\n", optopt);
				break;
		}
	}
	printf("Parsing input parameters - done!\n\n");

	while ((task = TASK_GetNext()) != NULL) {
		printf("  Task \"%s\"\n", task->Filename);
		ihex_read(task->Filename, &NewByte);

		TASK_Destroy();
	}

	page_t* temp = Page_GetRoot();
	while (temp != NULL) {
		printf("  PAGE i%d\n", temp->index);
		if (temp->DataFlag) {
			printf("  Address: 0x%08x\n", temp->Address);
			for (uint16_t i = 0; i < temp->PayloadSize; i++) {
				printf(" %02X", *(temp->Payload+i));
				if ((i & 15) == 15)
					printf("\n");
			}
		}
		temp = (page_t*) temp->Next;
	}



	Page_DestroyAll();

	printf("Program finishes\n");

	if (strPort) {
		free(strPort);
		strPort = NULL;
	}

	return 0;
}
