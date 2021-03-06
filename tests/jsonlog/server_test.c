#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>

#include "jsonlog/server.h"

/** Mock Functions **/

void __wrap_perror(const char *s) { 
  check_expected(s);
}

int __wrap_socket(int domain, int type, int protocol) {
  assert_int_equal(domain, AF_INET);
  assert_int_equal(type, SOCK_DGRAM);
  assert_int_equal(protocol, IPPROTO_UDP);
  return mock();
}

int __wrap_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  assert_int_equal(sockfd, 42);

  struct sockaddr_in *interface = (struct sockaddr_in*)addr;
  assert_int_equal(interface->sin_family, AF_INET);
  assert_int_equal(interface->sin_port, htons(123));
  assert_int_equal(interface->sin_addr.s_addr, htonl(INADDR_ANY));
  return mock();
}

ssize_t __wrap_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
  assert_int_equal(sockfd, 42);
  assert_int_equal(len, BUFLEN);
  assert_int_equal(flags, 0);
  assert_int_equal(*addrlen, sizeof(struct sockaddr_in));
  return mock();
}

/** Tests **/

static void tries_to_create_socket() {
  will_return(__wrap_socket, -1);
  expect_string(__wrap_perror, s, "Could not create datagram socket");
  assert_int_equal(3, start_server(42));
}

static void tries_to_bind_socket() {
  will_return(__wrap_socket, 42);
  will_return(__wrap_bind, -1);
  expect_string(__wrap_perror, s, "Could not bind to socket");
  assert_int_equal(4, start_server(123));
}

static void tries_to_read_from_socket() {
  will_return(__wrap_socket, 42);
  will_return(__wrap_bind, 1);
  will_return(__wrap_recvfrom, -1);
  expect_string(__wrap_perror, s, "Error reading socket");
  assert_int_equal(5, start_server(123));
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(tries_to_create_socket),
    cmocka_unit_test(tries_to_bind_socket),
    cmocka_unit_test(tries_to_read_from_socket),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
