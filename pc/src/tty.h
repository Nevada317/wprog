#include <stdint.h>
int tty_open(const char *portname);
int tty_close(int fd);

int tty_write(int descriptor, uint8_t* data, uint16_t length);
