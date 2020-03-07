/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : main
 * @created     : Saturday Feb 29, 2020 15:16:16 MST
 */

#include "serial.h"
#include <string.h>

char buffer[20];


int main() {
        int fd = serialport_init("/dev/ttyACM0", 9600);

        const char* send = "Hello";

        printf("%d\n", serialport_write(fd, send, strlen(send)));
        printf("%d\n", serialport_read(fd, buffer, strlen(send), 100));
        buffer[strlen(send)] = '\0';

        printf("%s\n", buffer);

        serialport_close(fd);


        return 0;
}
