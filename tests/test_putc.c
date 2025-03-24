/**
 * Test for putc syscall using CMocka
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>

/* Test putc functionality */
static void test_putc_syscall(void **state) {
    (void)state; // Mark as used to avoid compiler warning
    
    int result;
    
    // Test writing 'A'
    result = sys_putc('A');
    assert_int_equal(result, KORA_SUCCESS);
    
    // Test writing newline
    result = sys_putc('\n');
    assert_int_equal(result, KORA_SUCCESS);
}

/* Main test suite */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_putc_syscall),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
} 