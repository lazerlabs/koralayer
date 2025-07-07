/**
 * Test for brk and sbrk syscalls using CMocka
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <stdint.h>

static void test_sbrk_basic(void **state) {
    (void)state;
    void *start = sys_sbrk(0);
    assert_ptr_not_equal(start, (void *)-1);

    const ptrdiff_t inc = 4096;
    void *prev = sys_sbrk(inc);
    assert_ptr_equal(prev, start);

    char *mem = (char *)prev;
    mem[0] = 'A';
    mem[inc - 1] = 'Z';

    void *cur = sys_sbrk(0);
    assert_ptr_equal((char *)cur, (char *)start + inc);

    void *res = sys_brk(start);
    assert_ptr_equal(res, start);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_sbrk_basic),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
