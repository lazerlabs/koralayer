/**
 * Test for unlink syscall using CMocka
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "kora/syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TEST_FILE "/tmp/kora_test_unlink.txt"
#define TEST_CONTENT "This is content for the unlink test file."

/* Test fixture data structure */
struct test_data {
    int file_handle;
};

/* Test fixture setup and teardown */
static int setup(void **state) {
    // Allocate test data
    struct test_data *data = malloc(sizeof(struct test_data));
    if (data == NULL) {
        return -1;
    }
    
    // Initialize test data
    data->file_handle = -1;
    
    // Clean up any existing test file
    sys_unlink(TEST_FILE);
    
    // Set the test fixture data
    *state = data;
    return 0;
}

static int teardown(void **state) {
    struct test_data *data = *state;
    
    // Close file if open
    if (data->file_handle >= 0) {
        sys_close(data->file_handle);
    }
    
    // Clean up test file
    sys_unlink(TEST_FILE);
    
    // Free test data
    free(data);
    
    return 0;
}

/* Test unlink functionality */
static void test_unlink_success(void **state) {
    struct test_data *data = *state;
    int result;
    uint8_t type;
    
    // Create a test file
    data->file_handle = sys_open(TEST_FILE, KORA_O_WRONLY | KORA_O_CREAT);
    assert_true(data->file_handle >= 0);
    
    // Write some content
    result = sys_write(data->file_handle, TEST_CONTENT, strlen(TEST_CONTENT));
    assert_int_equal(result, strlen(TEST_CONTENT));
    
    // Close the file
    result = sys_close(data->file_handle);
    assert_int_equal(result, KORA_SUCCESS);
    data->file_handle = -1;
    
    // Verify the file exists
    result = sys_exists(TEST_FILE, &type);
    assert_true(result > 0);
    assert_int_equal(type, KORA_FILE_TYPE_REGULAR);
    
    // Now unlink (delete) the file
    result = sys_unlink(TEST_FILE);
    assert_int_equal(result, 0);
    
    // Verify the file no longer exists
    result = sys_exists(TEST_FILE, &type);
    assert_int_equal(result, 0);
}

/* Test unlink with non-existent file */
static void test_unlink_nonexistent(void **state) {
    (void)state; // Not used here
    int result;
    
    // Try to unlink a non-existent file
    result = sys_unlink("/tmp/kora_nonexistent_file_12345");
    assert_true(result < 0);
}

/* Test unlink with directory (should fail) */
static void test_unlink_directory(void **state) {
    (void)state; // Not used here
    int result;
    
    // Create a test directory
    result = sys_mkdir("/tmp/kora_test_unlink_dir");
    assert_int_equal(result, KORA_SUCCESS);
    
    // Try to unlink the directory (should fail)
    result = sys_unlink("/tmp/kora_test_unlink_dir");
    assert_true(result < 0);
    
    // Clean up
    sys_rmdir("/tmp/kora_test_unlink_dir");
}

/* Main test suite */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_unlink_success, setup, teardown),
        cmocka_unit_test_setup_teardown(test_unlink_nonexistent, setup, teardown),
        cmocka_unit_test_setup_teardown(test_unlink_directory, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
