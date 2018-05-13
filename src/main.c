#include <stdio.h>
#include "jsonlog/server.h"

#define PORT 8888   //The port on which to listen for incoming data

int main() {
  fprintf(stderr, "Starting jsonlog...\n");
  return start_server(PORT);
}
