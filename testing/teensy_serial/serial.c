#include "serial.h"

/*
 Opens a serial port for writing
 */
int serialport_init(const char *serialport, int baud) {
        struct termios toptions;
        int fd;

        // fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
        fd = open(serialport, O_RDWR | O_NONBLOCK);

        if (fd == -1) {
                perror("serialport_init: Unable to open port ");
                return -1;
        }

        // int iflags = TIOCM_DTR;
        // ioctl(fd, TIOCMBIS, &iflags);                 // turn on DTR
        // ioctl(fd, TIOCMBIC, &iflags);                // turn off DTR

        if (tcgetattr(fd, &toptions) < 0) {
                perror("serialport_init: Couldn't get term attributes");
                return -1;
        }

        speed_t brate = baud; // let you override switch below if needed

        switch (baud) {
        case 4800:
                brate = B4800;
                break;
        case 9600:
                brate = B9600;
                break;
#ifdef B14400
        case 14400:
                brate = B14400;
                break;
#endif
        case 19200:
                brate = B19200;
                break;
#ifdef B28800
        case 28800:
                brate = B28800;
                break;
#endif
        case 38400:
                brate = B38400;
                break;
        case 57600:
                brate = B57600;
                break;
        case 115200:
                brate = B115200;
                break;
        }
        cfsetispeed(&toptions, brate);
        cfsetospeed(&toptions, brate);

        // 8N1
        toptions.c_cflag &= ~PARENB;
        toptions.c_cflag &= ~CSTOPB;
        toptions.c_cflag &= ~CSIZE;
        toptions.c_cflag |= CS8;
        // no flow control
        toptions.c_cflag &= ~CRTSCTS;

        // toptions.c_cflag &= ~HUPCL; // disable hang-up-on-close to avoid
        // reset

        toptions.c_cflag |= CREAD | CLOCAL; // turn on READ & ignore ctrl lines
        toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

        toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
        toptions.c_oflag &= ~OPOST;                          // make raw

        // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
        toptions.c_cc[VMIN] = 0;
        toptions.c_cc[VTIME] = 0;
        // toptions.c_cc[VTIME] = 20;

        tcsetattr(fd, TCSANOW, &toptions);
        if (tcsetattr(fd, TCSAFLUSH, &toptions) < 0) {
                perror("init_serialport: Couldn't set term attributes");
                return -1;
        }

        return fd;
}

//
int serialport_close(int fd) { return close(fd); }

// Writes a single byte to the serialport
int serialport_writebyte(int fd, uint8_t b) {
        int n = write(fd, &b, 1);
        if (n != 1)
                return -1;
        return 0;
}

// Writes to the serial port specified amount of bytes and waits until the data
// has been writen
int serialport_write(int fd, const uint8_t *str, size_t bytes) {
        size_t n = write(fd, str, bytes);
        if (n != bytes) {
                perror("serialport_write: couldn't write whole string\n");
                return -1;
        }

        // Wait until all data has been written
        tcdrain(fd);
        return 0;
}

// Reads from the serial port one byte at a time (found that works best) with a
// specified time out in msec
int serialport_read(int fd, uint8_t *buf, size_t bytes, int timeout) {
        char b[1]; // read expects an array, so we give it a 1-byte array

        for (size_t i = 0U; i < bytes && timeout > 0; ++i) {

                int n = read(fd, b, 1);

                if (n == -1) {
                        return -1; // couldn't read
                }

                if (n == 0) {
                        delay(1); // wait 1 msec try again
                        timeout--;
                        if (timeout == 0){
                                printf("ERROR: Timeout\n");
                                return -2;
                        }
                        continue;
                }

#ifdef SERIALPORTDEBUG
                printf("serialport_read_until: i=%d, n=%d b='%c'\n", i, n,
                       b[0]); // debug
#endif
                buf[i] = b[0];
        }

        return 0;
}

// Flush macro
int serialport_flush(int fd) {
        sleep(2); // required to make flush work, for some reason TODO figure
                  // out why and fix this
        return tcflush(fd, TCIOFLUSH);
}
