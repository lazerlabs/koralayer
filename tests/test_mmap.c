/**
 * Test mmap, munmap and mprotect syscalls using CMocka
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <sys/mman.h>
#include <string.h>

static void test_mmap_basic(void **state) {
    (void)state;
    size_t len = 4096;
    char *mem = sys_mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    assert_ptr_not_equal(mem, (void *)-1);

    strcpy(mem, "kora");
    assert_string_equal(mem, "kora");

    int ret = sys_mprotect(mem, len, PROT_READ);
    assert_int_equal(ret, 0);

    ret = sys_munmap(mem, len);
    assert_int_equal(ret, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_mmap_basic),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
