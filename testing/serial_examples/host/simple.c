/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : serial
 * @created     : Friday Oct 04, 2019 21:35:58 MDT
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


/*
 * The following is a very simple read write
 * serial program. Note that this is pretty bad,
 * but it reads and writes serial information without
 * a higher level api
 */

// Open the serial port
int open_port(){
  // To return
  int fd;

  // Open the port
  // O_RDWR -- read and write
  // O_NOCTTY -- Sets this terminal as not the main terminal
  // O_NDELAY -- Doesnt wait for DCD
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd == -1)
  {
    perror("Cant open port /dev/ttyACM0 ");
  }
  else
    fcntl(fd, F_SETFL, 0);
  
  return fd;
}

int main() {
  int n;
  int fd = open_port();
    char buffer[20];
  if(fd != -1){
    n = write(fd, "ATZ\r", 4);
    if(n < 0)
      fputs("write() of 4 bytes failed!\n", stderr);

    fcntl(fd, F_SETFL, FNDELAY); // read is not blocking, just returns
    // fcntl(fd, F_SETFL, 0); // read doesn't return, blocks
    if(read(fd, buffer, 5) < 1)
      fputs("read() of 5 bytes failed\n", stderr);

    
    close(fd);
  }
  printf(buffer);
  return 1;
}
