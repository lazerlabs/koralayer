#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <errno.h>

static void test_sync_call(void **state) {
    (void)state;
    assert_int_equal(sys_sync(), 0);
}

static void test_reboot_stub(void **state) {
    (void)state;
    errno = 0;
    assert_int_equal(sys_reboot(0), -1);
}

static void test_mount_stub(void **state) {
    (void)state;
    errno = 0;
    assert_int_equal(sys_mount("src", "/tmp", "fs", 0, NULL), -1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sync_call),
        cmocka_unit_test(test_reboot_stub),
        cmocka_unit_test(test_mount_stub),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
