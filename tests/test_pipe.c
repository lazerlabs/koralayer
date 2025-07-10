#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <string.h>

static void test_pipe_rw(void **state) {
    (void)state;
    int fds[2];
    assert_int_equal(sys_pipe(fds), 0);

    const char *msg = "x";
    assert_int_equal(sys_write(fds[1], msg, 1), 1);

    char buf[2] = {0};
    assert_int_equal(sys_read(fds[0], buf, 1), 1);
    assert_string_equal(buf, msg);

    sys_close(fds[0]);
    sys_close(fds[1]);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_pipe_rw),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
