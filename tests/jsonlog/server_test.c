#include <stdarg.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdbool.h>
#include <stdio.h>

void __wrap_perror(const char *s) { 
  check_expected(s);
}

int __wrap_socket(int domain, int type, int protocol) {
  return mock();
}

#include "jsonlog/server.h"

static void should_return_if_cannot_create_socket(void **state) {
  will_return(__wrap_socket, -1);
  expect_string(__wrap_perror, s, "Could not create datagram socket");
  assert_int_equal(1, start_server(42));
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(should_return_if_cannot_create_socket),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
