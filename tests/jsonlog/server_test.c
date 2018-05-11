#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>
#include <stdbool.h>

#include "jsonlog/server.h"

static void should_exit_if_cannot_create_socket(void **state) {
  assert_true(false);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(should_exit_if_cannot_create_socket),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
