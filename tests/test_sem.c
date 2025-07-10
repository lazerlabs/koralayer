#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <semaphore.h>

static void test_sem_wait_post(void **state) {
    (void)state;
    sem_t sem;
    assert_int_equal(sem_init(&sem, 0, 0), 0);
    assert_int_equal(sys_sem_post(&sem), 0);
    assert_int_equal(sys_sem_wait(&sem), 0);
    sem_destroy(&sem);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sem_wait_post),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
