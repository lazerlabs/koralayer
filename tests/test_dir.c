/**
 * Directory operations test for KoraLayer using CMocka
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define TEST_DIR "/tmp/kora_test_dir"
#define TEST_SUBDIR "/tmp/kora_test_dir/subdir"
#define TEST_FILE "/tmp/kora_test_dir/test_file"
#define TEST_SYMLINK "/tmp/kora_test_dir/test_symlink"

/* Test fixture data structure */
struct test_data {
    int dir_handle;     // Directory handle for opendir tests
    int file_handle;    // File handle for file tests
    char buffer[256];   // Buffer for reading/writing 
};

/* Test fixture setup and teardown */
static int setup(void **state) {
    // Allocate test data
    struct test_data *data = malloc(sizeof(struct test_data));
    if (data == NULL) {
        return -1;
    }
    memset(data, 0, sizeof(struct test_data));
    
    // Clean up any left-over test directories from previous runs
    sys_unlink(TEST_SYMLINK);  
    sys_unlink(TEST_FILE);
    sys_rmdir(TEST_SUBDIR);
    sys_rmdir(TEST_DIR);
    
    // Create base test directory
    int result = sys_mkdir(TEST_DIR);
    if (result != KORA_SUCCESS) {
        free(data);
        return -1;
    }
    
    // Set the test fixture data
    *state = data;
    return 0;
}

static int teardown(void **state) {
    struct test_data *data = *state;
    
    // Close any open handles
    if (data->dir_handle >= 0) {
        sys_closedir(data->dir_handle);
    }
    
    if (data->file_handle >= 0) {
        sys_close(data->file_handle);
    }
    
    // Clean up test directories
    sys_unlink(TEST_SYMLINK);
    sys_unlink(TEST_FILE);
    sys_rmdir(TEST_SUBDIR);
    sys_rmdir(TEST_DIR);
    
    // Free test data
    free(data);
    
    return 0;
}

/* Test mkdir and rmdir functionality */
static void test_mkdir_rmdir(void **state) {
    int result;
    (void)state; // Not used here


    // Create subdirectory
    result = sys_mkdir(TEST_SUBDIR);
    assert_int_equal(result, KORA_SUCCESS);
    
    // Remove subdirectory
    result = sys_rmdir(TEST_SUBDIR);
    assert_int_equal(result, KORA_SUCCESS);
}

/* Test directory reading functionality */
static void test_directory_reading(void **state) {
    struct test_data *data = *state;
    int result;
    kora_dirent_t entry;
    int found_dot = 0;
    int found_dotdot = 0;
    int found_test_file = 0;

    // Create a test file
    data->file_handle = sys_open(TEST_FILE, KORA_O_CREAT | KORA_O_WRONLY);
    assert_true(data->file_handle >= 0);
    
    result = sys_write(data->file_handle, "test", 4);
    assert_int_equal(result, 4);
    
    sys_close(data->file_handle);
    data->file_handle = -1;
    
    // Open directory
    data->dir_handle = sys_opendir(TEST_DIR);
    assert_true(data->dir_handle >= 0);
    
    // Read directory entries
    while ((result = sys_readdir(data->dir_handle, &entry)) > 0) {
        printf("Found directory entry: %s (type: %d)\n", entry.name, entry.type);
        
        if (strcmp(entry.name, ".") == 0) {
            found_dot = 1;
            assert_int_equal(entry.type, KORA_DT_DIR);
        }
        else if (strcmp(entry.name, "..") == 0) {
            found_dotdot = 1;
            assert_int_equal(entry.type, KORA_DT_DIR);
        }
        else if (strcmp(entry.name, "test_file") == 0) {
            found_test_file = 1;
            assert_int_equal(entry.type, KORA_DT_REG);
        }
    }
    
    // Check that we found all expected entries
    assert_true(found_dot && found_dotdot && found_test_file);
    
    // Close directory
    result = sys_closedir(data->dir_handle);
    assert_int_equal(result, KORA_SUCCESS);
    data->dir_handle = -1;
    
    // Make sure rmdir fails on a file (expected behavior)
    result = sys_rmdir(TEST_FILE);
    assert_true(result != KORA_SUCCESS);
}

/* Test symlink and readlink functionality */
static void test_symlink(void **state) {
    struct test_data *data = *state;
    int result;

    // Create a test file
    data->file_handle = sys_open(TEST_FILE, KORA_O_CREAT | KORA_O_WRONLY);
    assert_true(data->file_handle >= 0);
    
    result = sys_write(data->file_handle, "test", 4);
    assert_int_equal(result, 4);
    
    sys_close(data->file_handle);
    data->file_handle = -1;
    
    // Create symlink
    result = sys_symlink(TEST_FILE, TEST_SYMLINK);
    assert_int_equal(result, KORA_SUCCESS);
    
    // Read symlink
    result = sys_readlink(TEST_SYMLINK, data->buffer, sizeof(data->buffer));
    assert_true(result >= 0);
    
    // Verify symlink target
    assert_string_equal(data->buffer, TEST_FILE);
}

/* Main test suite */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_mkdir_rmdir, setup, teardown),
        cmocka_unit_test_setup_teardown(test_directory_reading, setup, teardown),
        cmocka_unit_test_setup_teardown(test_symlink, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
} 
