#include <stdint.h>
int tty_open(const char *portname, uint32_t baud);
int tty_close(int fd);
