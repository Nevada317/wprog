#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "task.h"
#include "pages.h"

int main(int argc, char *argv[]) {
	printf("Program started\n");

	char* strPort = NULL;

	task_t* task;

	printf("Parsing input parameters...\n");
	// https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
	int opt;
	while((opt = getopt(argc, argv, ":p:f:e:F:E:sc")) != -1) {
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
			case 's':
				Page_Create_SignatureRead();
				break;
			case 'c':
				Page_Create_ChipErase();
				break;
			case 'f':
			case 'e':
			case 'F':
			case 'E':
				task = TASK_Create();
				if (opt == 'f')
					task->Action = Action_WriteFlash;
				if (opt == 'e')
					task->Action = Action_WriteEEPROM;
				if (opt == 'F')
					task->Action = Action_ReadFlash;
				if (opt == 'E')
					task->Action = Action_ReadEEPROM;
				task->Filename = malloc(strlen(optarg)+1);
				strcpy(task->Filename, optarg);
				printf("  Task created: %c:\"%s\"\n", opt, task->Filename);
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
		TASK_Destroy();
	}
	Page_DestroyAll();

	printf("Program finishes\n");

	if (strPort) {
		free(strPort);
		strPort = NULL;
	}

	return 0;
}
