/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : serial
 * @created     : Friday Oct 04, 2019 21:35:58 MDT
 */

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

volatile int running = 1;

void handle_sig(int signal) {
  printf("Recieved signal %d\n", signal);
  running = 0;
}

void* serial_write(void* fd_) {
  int fd = *(int*)fd_;
  char buffer[20];
  char data[20] = "Hello World\n";
  while(running) {
    printf("Recieving data: ");
    if(read(fd, buffer, 1) != 1)
      printf("Error reading data");
    sleep(1);
  }
  return NULL;
}

void* status(void* context) {
  while(running){
  printf("In here");
  sleep(1);
  }
  return NULL;
}

#define DEFAULT_BAUD  115200
#define DEFAULT_PORT "/dev/ttyACM0"

int main(int argc, char* argv[]) {
  
  signal(SIGINT, handle_sig);
  const char* port = DEFAULT_PORT;
  int baud = DEFAULT_BAUD;
  int fd;

  if(argc > 1)
    port = argv[1];

  if(argc > 2)
    baud = atoi(argv[2]);

  if(access(port, F_OK) == -1) {
    fprintf(stderr, "Cannot access file. Error %d : %s\n", errno, strerror(errno));
    exit(1);
  }

  printf("Successfully accessed file\n");
  struct termios toptions;
  
  fd = open(port, O_RDWR | O_NOCTTY | O_NODELAY);

  if(fd == -1) {
    fprintf(stderr, "Cannot open file. Error %d : %s\n", errno, strerror(errno));
    exit(1);
  }
  fcntl(fd, F_SETFL, 0)
  printf("Successfully opened file\n");

  if(tcgetattr(fd, &toptions) < 0) {
    fprintf(stderr, "init serial port error: %d : %s\n", errno, strerror(errno));
    exit(1);
  }
  printf("Successfully init serial port file\n");

  speed_t brate = baud;
  switch(baud) {
    case 4800:
      brate = B4800;
      break;
    case 9600:
      brate = B9600;
      break;
    case 19200:
      brate = B19200;
      break;
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

  toptions.c_cflag = toptions.c_cflag & ~PARENB & ~CSTOPB & ~CSIZE | CS8 & ~CRTSCTS | CREAD | CLOCAL;
  toptions.c_iflag = toptions.c_iflag & ~IXON & ~IXOFF & ~IXANY & ~ICANON & ~ECHO & ~ECHOE & ~ISIG;
  toptions.c_oflag &= ~OPOST;

  toptions.c_cc[VMIN] = 0;
  toptions.c_cc[VTIME] = 0;
  if(tcsetattr(fd, TCSANOW, &toptions) < 0) {
    fprintf(stderr, "Setting attributes failed: %d : %s\n", errno, strerror(errno));
    exit(1);
  }
  printf("Successfully set port file\n");

  pthread_t thread;
  if(pthread_create(&thread, NULL, serial_write, (void*)&fd)) {
    fprintf(stderr, "Error in creating thread");
    close(fd);
    exit(1);
  }

  pthread_join(thread, NULL);
  printf("Thread 1 done");

  close(fd);

}
