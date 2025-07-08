#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <sys/wait.h>

static void test_spawn_wait(void **state) {
    (void)state;
    char *argv[] = {"/bin/true", NULL};
    char *envp[] = {NULL};
    pid_t pid = sys_spawn("/bin/true", argv, envp);
    assert_true(pid > 0);

    int status = 0;
    pid_t wpid = sys_wait(pid, &status, 0);
    assert_int_equal(wpid, pid);
    assert_true(WIFEXITED(status));
    assert_int_equal(WEXITSTATUS(status), 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_spawn_wait),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
