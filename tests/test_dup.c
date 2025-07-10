#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <string.h>

static void test_dup_basic(void **state) {
    (void)state;
    int fds[2];
    assert_int_equal(sys_pipe(fds), 0);

    int dupfd = sys_dup(fds[1]);
    assert_true(dupfd >= 0);

    const char *msg = "y";
    assert_int_equal(sys_write(dupfd, msg, 1), 1);

    char buf[2] = {0};
    assert_int_equal(sys_read(fds[0], buf, 1), 1);
    assert_string_equal(buf, msg);

    sys_close(fds[0]);
    sys_close(fds[1]);
    sys_close(dupfd);
}

static void test_dup2_basic(void **state) {
    (void)state;
    int fds[2];
    assert_int_equal(sys_pipe(fds), 0);

    int dupfd = sys_dup2(fds[1], fds[0]);
    assert_true(dupfd >= 0);

    const char *msg = "z";
    assert_int_equal(sys_write(dupfd, msg, 1), 1);

    char buf[2] = {0};
    assert_int_equal(sys_read(fds[0], buf, 1), 1);
    assert_string_equal(buf, msg);

    sys_close(fds[0]);
    sys_close(fds[1]);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_dup_basic),
        cmocka_unit_test(test_dup2_basic),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
