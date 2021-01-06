#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "tty.h"

static int set_interface_attribs(int fd);

int tty_open(const char *portname) {
	int fd;
	fd = open(portname, O_RDWR | O_NOCTTY);
	//     set_mincount(fd, 1); // set to pure timed read
	if (fd < 0) {
		printf("Error opening %s: %s\n", portname, strerror(errno));
		return -1;
	}
	if (set_interface_attribs(fd))
		return -1;

	uint8_t buf[30];
	int rdlen;
	buf[0] = 0x40;
	write(fd, buf, 1);
	for (uint8_t i = 0; i<255; i++) {
		buf[0] = i;
		write(fd, buf, 1);
		rdlen = read(fd, buf, 5);
		if (rdlen){
			printf("(%02x)  Read %d bytes", i, rdlen);
			for (uint8_t ch = 0; ch < rdlen; ch++)
				printf(" %02x", buf[ch]);
			printf("\n");
		}
	}

	return fd;
}



static int set_interface_attribs(int fd) {
	struct termios tty;

	if (tcgetattr(fd, &tty) < 0) {
		printf("Error from tcgetattr: %s\n", strerror(errno));
		return -1;
	}
	if (cfsetospeed(&tty, (speed_t)B57600))
		return -1;
	if (cfsetispeed(&tty, (speed_t)B57600))
		return -1;

	cfmakeraw(&tty);

	tty.c_cflag |= CLOCAL;  // Ignore modem controls
	tty.c_cflag |= CREAD;   // Enable receiver

	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;     // 8-bit characters

	tty.c_cflag &= ~PARENB;     // no parity bit
//     tty.c_cflag |= PARENB;    // parity enabled
//     tty.c_cflag &= PARODD;    // NOT odd = even

	tty.c_cflag &= ~CSTOPB;     // only need 1 stop bit
// 	tty.c_cflag |= CSTOPB;     // 2 stop bit

	tty.c_cflag &= ~CRTSCTS;   // no hardware flowcontrol
// 	tty.c_cflag &= ~HUPCL; // Do not raise DTR

	// Use custom baud
	// 	tty.c_cflag &= ~CBAUD;
	// 	tty.c_cflag |= CBAUDEX;

// 	tty.c_cflag |= CBAUD;
// 	tty.c_cflag &= ~CBAUDEX;


	/* setup for non-canonical mode */
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty.c_oflag &= ~OPOST;

	/* fetch bytes as they become available */
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 1;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("Error from tcsetattr: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int tty_close(int fd) {
	close(fd);
	return 0;
}

/*
int getttychar(int fd, int* owner, int color, char* Start) {
	unsigned char buf[128];
	int rdlen;
	int started = 0;
	rdlen = 1;
	if (*owner == fd) {
		started = 1;
	}
	while (rdlen > 0) {
		rdlen = read(fd, &buf, 128);
		if (rdlen > 0) {
			if ((*owner != fd) && (*owner != 0)) {
				if (color > 0)
					printf("\e[0m");
				*owner = 0;
			}
			unsigned char* pwr;
			pwr = &buf[0];
			if (started == 0) {
				if (color > 0)
					printf("%s\e[3%d;1m", Start, color);
				started = 1;
				*owner = fd;
			}

			for (int i = 0; i<rdlen; i++) {
				printf("%02x ", *(pwr++));
			}
		}
	}

	fflush(stdout);
	if (rdlen < 0) {
		printf("Error from read: %d: %s\n", rdlen, strerror(errno));
		return -2;
	}
	return 0;
}*/

