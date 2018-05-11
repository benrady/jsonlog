#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "writer.h"

#define BUFLEN 1024 * 64  //Max length of buffer
#define PORT 8888   //The port on which to listen for incoming data

void die(char *s)
{
  perror(s);
  exit(1);
}

int start_server(void)
{
  struct sockaddr_in si_me, si_other;

  socklen_t slen = sizeof(si_other);
  int recv_len;
  char buf[BUFLEN];

  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == sockfd) die("Could not create datagram socket");

  // zero out the structure
  memset((char *) &si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
    die("Could not bind to socket");
  }

  fprintf(stderr, "Ready for data...\n");

  while(1) {
    recv_len = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
    if (-1 == recv_len) {
      die("Error reading socket");
    }

    //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
    write_message("default", buf);
  }

  return 0;
}
