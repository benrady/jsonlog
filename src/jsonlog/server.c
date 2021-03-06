#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "server.h"
#include "writer.h"

int recv_json(int sockfd, char buf[], struct sockaddr *addr_sender) {
  socklen_t slen = sizeof(struct sockaddr_in);
  int recv_len = recvfrom(sockfd, buf, BUFLEN, 0, addr_sender, &slen);
  if (-1 == recv_len) {
    perror("Error reading socket");
    return 5;
  }

  //printf("Received packet from %s:%d\n", inet_ntoa(addr_sender.sin_addr), ntohs(addr_sender.sin_port));
  write_message("default", buf);
  return 0;
}

int start_server(int port)
{
  struct sockaddr_in addr_interface, addr_sender;
  char buf[BUFLEN];

  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == sockfd) {
    perror("Could not create datagram socket");
    return 3;
  }

  // zero out the structure
  memset((char *) &addr_interface, 0, sizeof(addr_interface));

  addr_interface.sin_family = AF_INET;
  addr_interface.sin_port = htons(port);
  addr_interface.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr*)&addr_interface, sizeof(addr_interface) ) == -1) {
    perror("Could not bind to socket");
    return 4;
  }

  int retval;
  do {
    retval = recv_json(sockfd, buf, (struct sockaddr*)&addr_sender);
  } while(retval == 0);
  return retval;
}

