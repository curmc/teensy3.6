/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : serial
 * @created     : Wednesday Nov 27, 2019 21:17:31 MST
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <memory.h>
#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

// Millisecond delay (like arduino)
#define delay(val) usleep(1000 * val)
#define DEFAULT_BAUD 9600

/*
 * Serial
 * communication
 * functions
 */
int serialport_init(const char *serialport, int baud);
int serialport_close(int fd);
int serialport_writebyte(int fd, uint8_t b);
int serialport_write(int fd, const uint8_t *str, size_t bytes);
int serialport_read(int fd, uint8_t *buf, size_t bytes, int timeout);
int serialport_flush(int fd);

#ifdef __cplusplus
}
#endif

