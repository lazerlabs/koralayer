#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>

static void test_clock_gettime_call(void **state) {
    (void)state;
    struct timespec ts;
    assert_int_equal(sys_clock_gettime(CLOCK_REALTIME, &ts), 0);
    assert_true(ts.tv_sec > 0);
}

static void test_gettimeofday_call(void **state) {
    (void)state;
    struct timeval tv;
    assert_int_equal(sys_gettimeofday(&tv, NULL), 0);
    assert_true(tv.tv_sec > 0);
}

static void test_nanosleep_call(void **state) {
    (void)state;
    struct timespec start, end, req = {0, 100000000};
    assert_int_equal(sys_clock_gettime(CLOCK_MONOTONIC, &start), 0);
    assert_int_equal(sys_nanosleep(&req, NULL), 0);
    assert_int_equal(sys_clock_gettime(CLOCK_MONOTONIC, &end), 0);
    assert_true((end.tv_sec > start.tv_sec) ||
                (end.tv_sec == start.tv_sec && end.tv_nsec >= start.tv_nsec + 100000000));
}

static void test_sleep_wrapper(void **state) {
    (void)state;
    struct timespec start, end;
    assert_int_equal(sys_clock_gettime(CLOCK_MONOTONIC, &start), 0);
    assert_int_equal(sys_sleep(1), 0);
    assert_int_equal(sys_clock_gettime(CLOCK_MONOTONIC, &end), 0);
    assert_true(end.tv_sec >= start.tv_sec + 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_clock_gettime_call),
        cmocka_unit_test(test_gettimeofday_call),
        cmocka_unit_test(test_nanosleep_call),
        cmocka_unit_test(test_sleep_wrapper),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
