/**
 * Test for rename syscall using CMocka
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <string.h>
#include <stdlib.h>

#define SRC_FILE "/tmp/kora_test_rename_src.txt"
#define DST_FILE "/tmp/kora_test_rename_dst.txt"

struct test_data {
    int fd;
};

static int setup(void **state) {
    struct test_data *data = malloc(sizeof(struct test_data));
    if (!data) {
        return -1;
    }
    data->fd = -1;
    sys_unlink(SRC_FILE);
    sys_unlink(DST_FILE);
    *state = data;
    return 0;
}

static int teardown(void **state) {
    struct test_data *data = *state;
    if (data->fd >= 0) {
        sys_close(data->fd);
    }
    sys_unlink(SRC_FILE);
    sys_unlink(DST_FILE);
    free(data);
    return 0;
}

static void test_rename_basic(void **state) {
    struct test_data *data = *state;
    int ret;
    uint8_t type;

    data->fd = sys_open(SRC_FILE, KORA_O_WRONLY | KORA_O_CREAT | KORA_O_TRUNC);
    assert_true(data->fd >= 0);

    ret = sys_write(data->fd, "rename", 6);
    assert_int_equal(ret, 6);

    ret = sys_close(data->fd);
    assert_int_equal(ret, KORA_SUCCESS);
    data->fd = -1;

    ret = sys_rename(SRC_FILE, DST_FILE);
    assert_int_equal(ret, 0);

    ret = sys_exists(DST_FILE, &type);
    assert_true(ret > 0);
    assert_int_equal(type, KORA_FILE_TYPE_REGULAR);

    ret = sys_exists(SRC_FILE, &type);
    assert_int_equal(ret, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_rename_basic, setup, teardown),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
