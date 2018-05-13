#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "server.h"
#include "writer.h"

#define BUFLEN 1024 * 64  //Max length of buffer

int start_server(int port)
{
  struct sockaddr_in si_me, si_other;

  socklen_t slen = sizeof(si_other);
  int recv_len;
  char buf[BUFLEN];

  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == sockfd) {
    perror("Could not create datagram socket");
    return 1;
  }

  // zero out the structure
  memset((char *) &si_me, 0, sizeof(si_me));

  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1) {
    perror("Could not bind to socket");
    return 1;
  }

  fprintf(stderr, "Ready for data...\n");

  while(1) {
    recv_len = recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen);
    if (-1 == recv_len) {
      perror("Error reading socket");
      return 1;
    }

    //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
    write_message("default", buf);
  }

  return 0;
}
