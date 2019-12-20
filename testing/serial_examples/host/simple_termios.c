/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : simple_termios
 * @created     : Friday Oct 04, 2019 22:30:13 MDT
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>

volatile int running = 1;

void set_baud(struct termios term, int baud) {
  cfsetispeed(&term, baud);
  cfsetospeed(&term, baud);
}

void signal_handel(int signal) {
  puts("Recieved signal");
  running = 0;
}

int open_port() {
  int fd;
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
  if(fd == -1)
    perror("Cant open port /dev/ttyACM0");
  else
    fcntl(fd, F_SETFL, 0);
  return fd;
}

int main() {
  struct termios previous_options;
  struct termios options;

  int fd;

  // Create a new file opening
  fd = open_port();

  // Set get previous attributes to save for the end
  tcgetattr(fd, &previous_options);
  // This are the attributes we'll change
  tcgetattr(fd, &options);

  set_baud(options, B9600);

  // CLOCAL -- Do not change owner of the port
  // CREAD -- Enable reciever
  options.c_cflag |= (CLOCAL | CREAD);

  // Set parity checking
  // For now, no parity
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;


  // Canonical
  /* Canonical is line oriented, input characters are put into
   * buffer and edited until carriage return (\n) or line feed
   * is recieved
   */
  /** options.c_lflag |= (ICANON | ECHO | ECHOE); */
  
  // Raw Input
  /* Raw input is unprocessed. Input characters are passed
   * exactly as they are recieved when recieved
   */
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  // Do this if parity is enabled
  /** options.c_iflag = (INPCK | ISTRIP); */

  /* Flow Control:
   * Often we need to regulate the flow of data when transfering
   * data between two serial interfaces. Due to limitations
   * in immediate serial communication link
   *
   * software flow: every transaction starts with a special start
   * character (XON) or stop character (XOFF). 
   *
   * hardware flow: every transaction starts with a CTS and RTS
   * signal instead of character
   */
  /** options.c_iflag |= (IXON | IXOFF | IXANY); // Turn flow control on */
  options.c_iflag &= ~(IXON | IXOFF | IXANY); // turn it off

  // Output options
  // Processed output -- if false, all other oflags are ignored
  options.c_oflag &= ~OPOST; // choosing raw output

  options.c_cc[VMIN] = 0; 
  options.c_cc[VTIME] = 10;


  // TCSANOW -- sets the changes without waiting for data to transfer
  if(tcsetattr(fd, TCSANOW, &options) == -1){
    close(fd);
    printf("Error");
    exit(1);
  }

  signal(SIGINT, signal_handel);
  char recv_buffer[20] = "yo";
  char send_buffer[20] = "Hello There!";
  int val;
  while(running){
    puts("Recieving data");
    val = read(fd, recv_buffer, 6);
    printf("%d\n", val);
    printf("%s\n", recv_buffer);
  }

  if(tcsetattr(fd, TCSANOW, &previous_options) == -1)
    printf("Error2");

  return 1;
}

