/**
 * Test program for file information system calls
 * 
 * Tests for sys_get_file_info, sys_get_fd_info, and sys_exists
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <unistd.h>  // For getcwd

// Use a subdirectory of the current directory for tests
#define TEST_BASE_DIR "/tmp/kora_test_fileinfo"
#define TEST_DIR TEST_BASE_DIR "/test_dir"
#define TEST_FILE TEST_DIR "/test_file.txt"
#define TEST_SYMLINK TEST_DIR "/test_symlink"
#define NONEXISTENT_PATH TEST_BASE_DIR "/nonexistent"

// Test file content
#define TEST_CONTENT "This is test content for file information testing.\n"

/* Test fixture data */
struct test_data {
    kora_file_info_t file_info;
};

/* Setup and teardown for all tests */
static int global_setup(void **state) {
    struct test_data *data = malloc(sizeof(struct test_data));
    assert_non_null(data);
    
    // Clean up any existing test directories from previous runs
    sys_unlink(TEST_SYMLINK);
    sys_unlink(TEST_FILE);
    sys_rmdir(TEST_DIR);
    sys_rmdir(TEST_BASE_DIR);
    
    // Create the base test directory
    int ret = sys_mkdir(TEST_BASE_DIR);
    if (ret != 0) {
        printf("Failed to create base directory '%s': %d\n", TEST_BASE_DIR, ret);
        free(data);
        return -1;
    }
    
    // Create the test directory
    ret = sys_mkdir(TEST_DIR);
    if (ret != 0) {
        printf("Failed to create test directory '%s': %d\n", TEST_DIR, ret);
        sys_rmdir(TEST_BASE_DIR);
        free(data);
        return -1;
    }
    
    // Create and write to the test file
    int fd = sys_open(TEST_FILE, KORA_O_WRONLY | KORA_O_CREAT);
    if (fd < 0) {
        printf("Failed to create test file '%s': %d\n", TEST_FILE, fd);
        sys_rmdir(TEST_DIR);
        sys_rmdir(TEST_BASE_DIR);
        free(data);
        return -1;
    }
    
    ret = sys_write(fd, TEST_CONTENT, strlen(TEST_CONTENT));
    if (ret != strlen(TEST_CONTENT)) {
        printf("Failed to write content to test file: %d\n", ret);
        sys_close(fd);
        sys_unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        sys_rmdir(TEST_BASE_DIR);
        free(data);
        return -1;
    }
    
    sys_close(fd);
    
    // Create the symlink
    ret = sys_symlink(TEST_FILE, TEST_SYMLINK);
    if (ret != 0) {
        printf("Failed to create symlink '%s': %d\n", TEST_SYMLINK, ret);
        sys_unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        sys_rmdir(TEST_BASE_DIR);
        free(data);
        return -1;
    }
    
    // Print current directory for debugging
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    }
    
    printf("Setup complete: Created test directory at %s\n", TEST_DIR);
    
    *state = data;
    return 0;
}

static int global_teardown(void **state) {
    struct test_data *data = *state;
    
    // Cleanup test files
    printf("Cleaning up test files...\n");
    sys_unlink(TEST_SYMLINK);
    sys_unlink(TEST_FILE);
    sys_rmdir(TEST_DIR);
    sys_rmdir(TEST_BASE_DIR);
    
    free(data);
    return 0;
}

/* Test functions */
static void test_fileinfo_exists_directory(void **state) {
    (void)state; // Mark as used to avoid compiler warning
    
    uint8_t type;
    int ret = sys_exists(TEST_DIR, &type);
    printf("sys_exists(%s) returned %d, type=%d\n", TEST_DIR, ret, type);
    
    assert_true(ret > 0);
    assert_int_equal(type, KORA_FILE_TYPE_DIRECTORY);
}

static void test_fileinfo_exists_regular_file(void **state) {
    (void)state; // Mark as used to avoid compiler warning
    
    uint8_t type;
    int ret = sys_exists(TEST_FILE, &type);
    printf("sys_exists(%s) returned %d, type=%d\n", TEST_FILE, ret, type);
    
    assert_true(ret > 0);
    assert_int_equal(type, KORA_FILE_TYPE_REGULAR);
}

static void test_fileinfo_exists_symlink(void **state) {
    (void)state; // Mark as used to avoid compiler warning
    
    uint8_t type;
    int ret = sys_exists(TEST_SYMLINK, &type);
    printf("sys_exists(%s) returned %d, type=%d\n", TEST_SYMLINK, ret, type);
    
    assert_true(ret > 0);
    assert_int_equal(type, KORA_FILE_TYPE_SYMLINK);
}

static void test_fileinfo_exists_nonexistent(void **state) {
    (void)state; // Mark as used to avoid compiler warning
    
    uint8_t type;
    int ret = sys_exists(NONEXISTENT_PATH, &type);
    printf("sys_exists(%s) returned %d, type=%d\n", NONEXISTENT_PATH, ret, type);
    
    assert_int_equal(ret, 0);
}

static void test_fileinfo_get_file_info_regular(void **state) {
    struct test_data *data = *state;
    
    int ret = sys_get_file_info(TEST_FILE, &data->file_info);
    printf("sys_get_file_info(%s) returned %d, type=%d, size=%llu\n", 
           TEST_FILE, ret, data->file_info.type, data->file_info.size);
    
    assert_int_equal(ret, 0);
    assert_int_equal(data->file_info.type, KORA_FILE_TYPE_REGULAR);
    assert_int_equal(data->file_info.size, strlen(TEST_CONTENT));
}

static void test_fileinfo_get_fd_info_regular(void **state) {
    struct test_data *data = *state;
    
    int fd = sys_open(TEST_FILE, KORA_O_RDONLY);
    printf("sys_open(%s) returned fd=%d\n", TEST_FILE, fd);
    assert_true(fd > -1);
    
    int ret = sys_get_fd_info(fd, &data->file_info);
    printf("sys_get_fd_info(%d) returned %d, type=%d, size=%llu\n", 
           fd, ret, data->file_info.type, data->file_info.size);
    
    assert_int_equal(ret, 0);
    assert_int_equal(data->file_info.type, KORA_FILE_TYPE_REGULAR);
    assert_int_equal(data->file_info.size, strlen(TEST_CONTENT));
    
    sys_close(fd);
}

static void test_fileinfo_get_fd_info_invalid(void **state) {
    struct test_data *data = *state;
    
    int ret = sys_get_fd_info(-1, &data->file_info);
    printf("sys_get_fd_info(-1) returned %d\n", ret);
    
    assert_true(ret < 0);
}

/* Main test suite */
int main(void) {
    // Run setup and teardown for each test
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_fileinfo_exists_directory, global_setup, global_teardown),
        cmocka_unit_test_setup_teardown(test_fileinfo_exists_regular_file, global_setup, global_teardown),
        cmocka_unit_test_setup_teardown(test_fileinfo_exists_symlink, global_setup, global_teardown),
        cmocka_unit_test_setup_teardown(test_fileinfo_exists_nonexistent, global_setup, global_teardown),
        cmocka_unit_test_setup_teardown(test_fileinfo_get_file_info_regular, global_setup, global_teardown),
        cmocka_unit_test_setup_teardown(test_fileinfo_get_fd_info_regular, global_setup, global_teardown),
        cmocka_unit_test_setup_teardown(test_fileinfo_get_fd_info_invalid, global_setup, global_teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
} 
