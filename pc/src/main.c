#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	printf("Program started\n");

	char* strPort = NULL;

	printf("Parsing input parameters...\n");
	// https://www.geeksforgeeks.org/getopt-function-in-c-to-parse-command-line-arguments/
	int opt;
	while((opt = getopt(argc, argv, ":p:")) != -1) {
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
			case ':':
				printf("  Option %c needs a value\n", opt);
				break;
			case '?':
				printf("  Unknown option: %c\n", optopt);
				break;
		}
	}
	printf("Parsing input parameters - done!\n\n");

	printf("Program finishes\n");

	if (strPort) {
		free(strPort);
		strPort = NULL;
	}

	return 0;
}
