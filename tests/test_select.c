#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <string.h>

static void test_select_pipe(void **state) {
    (void)state;
    int fds[2];
    assert_int_equal(sys_pipe(fds), 0);

    fd_set r;
    FD_ZERO(&r);
    FD_SET(fds[0], &r);
    struct timeval tv = {0, 0};
    assert_int_equal(sys_select(fds[0]+1, &r, NULL, NULL, &tv), 0);

    assert_int_equal(sys_write(fds[1], "a", 1), 1);

    FD_ZERO(&r);
    FD_SET(fds[0], &r);
    tv.tv_sec = 0; tv.tv_usec = 0;
    assert_int_equal(sys_select(fds[0]+1, &r, NULL, NULL, &tv), 1);
    assert_true(FD_ISSET(fds[0], &r));

    char buf[2] = {0};
    assert_int_equal(sys_read(fds[0], buf, 1), 1);
    assert_string_equal(buf, "a");

    sys_close(fds[0]);
    sys_close(fds[1]);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_select_pipe),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
