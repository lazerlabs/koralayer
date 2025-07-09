#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <unistd.h>

static void test_getpid_getppid(void **state) {
    (void)state;
    assert_int_equal(sys_getpid(), getpid());
    assert_int_equal(sys_getppid(), getppid());
}

static void test_yield_call(void **state) {
    (void)state;
    int ret = sys_yield();
    assert_int_equal(ret, 0);
}

static void test_setpriority_call(void **state) {
    (void)state;
    pid_t pid = sys_getpid();
    int ret = sys_setpriority(pid, 0);
    assert_int_equal(ret, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_getpid_getppid),
        cmocka_unit_test(test_yield_call),
        cmocka_unit_test(test_setpriority_call),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
