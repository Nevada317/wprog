#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	printf("Program started\n");
	int opt;
	while((opt = getopt(argc, argv, ":p:")) != -1) {
		switch (opt) {
			case 'p':
				printf("Port is %s\n", optarg);
				break;
			case ':':
				printf("option %c needs a value\n", opt);
				break;
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
		}
	}

	printf("Program finishes\n");
	return 0;
}
