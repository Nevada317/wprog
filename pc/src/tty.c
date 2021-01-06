#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "tty.h"

static int set_interface_attribs(int fd, uint32_t speed);

int tty_open(const char *portname, uint32_t baud) {
	int fd;
	fd = open(portname, O_RDWR | O_NOCTTY);
	//     set_mincount(fd, 1); // set to pure timed read
	if (fd < 0) {
		printf("Error opening %s: %s\n", portname, strerror(errno));
		return -1;
	}
	if (set_interface_attribs(fd, baud))
		return -1;

	write(fd, "\x02", 1);
	sleep(1);
	write(fd, "\x02", 1);
	sleep(1);
	write(fd, "\x02", 1);

	return fd;
}



static int set_interface_attribs(int fd, uint32_t baud) {
	struct termios tty;

	if (tcgetattr(fd, &tty) < 0) {
		printf("Error from tcgetattr: %s\n", strerror(errno));
		return -1;
	}

	tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;         /* 8-bit characters */
	tty.c_cflag &= ~PARENB;     /* no parity bit */
//     tty.c_cflag |= PARENB;     /* parity enabled */
//     tty.c_cflag &= PARODD;     /* NOT odd = even */

//    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
	tty.c_cflag |= CSTOPB;     /* 2 stop bit */

	tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */
	tty.c_cflag &= ~HUPCL; // Do not raise DTR

	// Use custom baud
	tty.c_cflag &= ~CBAUD;
	tty.c_cflag |= CBAUDEX;


	/* setup for non-canonical mode */
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	tty.c_oflag &= ~OPOST;

	/* fetch bytes as they become available */
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 0;

	if (tcsetattr(fd, TCSANOW, &tty) != 0) {
		printf("Error from tcsetattr: %s\n", strerror(errno));
		return -1;
	}
	cfsetospeed(&tty, (speed_t)baud);
	cfsetispeed(&tty, (speed_t)baud);
	return 0;
}

int tty_close(int fd) {
	close(fd);
	return 0;
}

