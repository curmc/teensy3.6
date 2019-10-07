/**
 * @author      : theo (theo@$HOSTNAME)
 * @file        : client
 * @created     : Saturday Oct 05, 2019 18:25:56 MDT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <unistd.h>

#define SERVER_ADDR "192.168.1.2"
#define BUFFER_SIZE 1024
#define PORT        80


int main() {
  int sockfd;
  struct sockaddr_in dest;
  char buffer[BUFFER_SIZE];

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Socket");
    exit(errno);
  }

  struct ifreq ifr;
  memset(&ifr, 0, sizeof(ifr));
  snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "enp0s20f0u2");
  if(setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void*)&ifr, sizeof(ifr)) < 0) {
    perror("Interface");
    exit(errno);
  }

  bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(PORT);
  if(inet_aton(SERVER_ADDR, &dest.sin_addr.s_addr) == 0) {
    perror(SERVER_ADDR);
    exit(errno);
  }

  if(connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0) {
    perror("Connect ");
    exit(errno);
  }

  close(sockfd);

  return 1;
  
}
