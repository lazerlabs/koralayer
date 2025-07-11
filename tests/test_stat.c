/**
 * Tests for stat family and related syscalls
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define TEST_DIR "/tmp/kora_test_stat"
#define TEST_FILE TEST_DIR "/test.txt"
#define TEST_LINK TEST_DIR "/test_link"
#define TEST_HARD TEST_DIR "/test_hard"
#define CONTENT "stat test content"

struct test_data {
    int fd;
    char cwd[256];
};

static int setup(void **state) {
    struct test_data *data = malloc(sizeof(struct test_data));
    if (!data) return -1;
    data->fd = -1;
    sys_unlink(TEST_HARD);
    sys_unlink(TEST_LINK);
    sys_unlink(TEST_FILE);
    sys_rmdir(TEST_DIR);
    sys_mkdir(TEST_DIR);
    data->fd = sys_open(TEST_FILE, KORA_O_WRONLY | KORA_O_CREAT | KORA_O_TRUNC);
    if (data->fd < 0) return -1;
    sys_write(data->fd, CONTENT, strlen(CONTENT));
    sys_close(data->fd);
    data->fd = sys_open(TEST_FILE, KORA_O_RDONLY);
    sys_symlink(TEST_FILE, TEST_LINK);
    sys_getcwd(data->cwd, sizeof(data->cwd));
    *state = data;
    return 0;
}

static int teardown(void **state) {
    struct test_data *data = *state;
    if (data->fd >= 0) sys_close(data->fd);
    sys_chdir(data->cwd);
    sys_unlink(TEST_HARD);
    sys_unlink(TEST_LINK);
    sys_unlink(TEST_FILE);
    sys_rmdir(TEST_DIR);
    free(data);
    return 0;
}

static void test_stat_file(void **state) {
    (void)state;
    kora_stat_t st;
    assert_int_equal(sys_stat(TEST_FILE, &st), 0);
    assert_true((st.mode & S_IFMT) == S_IFREG);
    assert_int_equal(st.size, strlen(CONTENT));
}

static void test_fstat_file(void **state) {
    struct test_data *data = *state;
    kora_stat_t st;
    assert_int_equal(sys_fstat(data->fd, &st), 0);
    assert_true((st.mode & S_IFMT) == S_IFREG);
    assert_int_equal(st.size, strlen(CONTENT));
}

static void test_lstat_link(void **state) {
    (void)state;
    kora_stat_t st;
    assert_int_equal(sys_lstat(TEST_LINK, &st), 0);
    assert_true((st.mode & S_IFMT) == S_IFLNK);
}

static void test_link_and_utime(void **state) {
    (void)state;
    kora_stat_t st;
    assert_int_equal(sys_link(TEST_FILE, TEST_HARD), 0);
    assert_int_equal(sys_stat(TEST_HARD, &st), 0);
    assert_int_equal(st.size, strlen(CONTENT));
    uint64_t new_time = 1234567890ULL;
    assert_int_equal(sys_utime(TEST_HARD, new_time), 0);
    assert_int_equal(sys_stat(TEST_HARD, &st), 0);
    assert_int_equal(st.mtime, new_time);
    sys_unlink(TEST_HARD);
}

static void test_chdir_getcwd(void **state) {
    struct test_data *data = *state;
    char buf[256];
    assert_int_equal(sys_chdir(TEST_DIR), 0);
    assert_int_equal(sys_getcwd(buf, sizeof(buf)), 0);
    assert_string_equal(buf, TEST_DIR);
    sys_chdir(data->cwd);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_stat_file, setup, teardown),
        cmocka_unit_test_setup_teardown(test_fstat_file, setup, teardown),
        cmocka_unit_test_setup_teardown(test_lstat_link, setup, teardown),
        cmocka_unit_test_setup_teardown(test_link_and_utime, setup, teardown),
        cmocka_unit_test_setup_teardown(test_chdir_getcwd, setup, teardown),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
