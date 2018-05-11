#include <stdio.h>
#include "jsonlog/server.h"

int main() {
  fprintf(stderr, "Starting jsonlog...\n");
  return start_server();
}
