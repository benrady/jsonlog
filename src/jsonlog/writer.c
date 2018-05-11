#include <stdio.h>

#include <arpa/inet.h>
int write_message(const char* key, const char* message) {
  printf("Key: %s, Data: %s\n" , key, message);
  fflush(stdout);
  return 0;
}
