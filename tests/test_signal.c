#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <signal.h>

static volatile int handled = 0;

static void sigusr1_handler(int signo) {
    (void)signo;
    handled = 1;
    sys_sigreturn();
}

static void test_signal_send(void **state) {
    (void)state;
    handled = 0;
    assert_ptr_not_equal(sys_signal(SIGUSR1, sigusr1_handler), SIG_ERR);
    assert_int_equal(sys_kill(sys_getpid(), SIGUSR1), 0);
    struct timespec ts = {0, 1000000};
    sys_nanosleep(&ts, NULL);
    assert_true(handled);
}

static volatile int alarmed = 0;

static void alarm_handler(int signo) {
    (void)signo;
    alarmed = 1;
    sys_sigreturn();
}

static void test_signal_interrupt_sleep(void **state) {
    (void)state;
    alarmed = 0;
    assert_ptr_not_equal(sys_signal(SIGALRM, alarm_handler), SIG_ERR);
    struct itimerval it = { {0,0}, {0,100000} };
    assert_int_equal(sys_setitimer(ITIMER_REAL, &it, NULL), 0);
    struct timespec req = {1, 0};
    int ret = sys_nanosleep(&req, NULL);
    assert_true(ret != 0);
    assert_true(alarmed);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_signal_send),
        cmocka_unit_test(test_signal_interrupt_sleep),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
