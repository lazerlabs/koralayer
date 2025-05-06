/**
 * I/O operations test for KoraLayer using CMocka
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <kora/syscalls.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FILE "/tmp/kora_test_io.txt"
#define TEST_DATA "Hello, KoraOS!\n"
#define BUFFER_SIZE 128

/* Test fixture data structure */
struct test_data {
    int file_handle;
    char buffer[BUFFER_SIZE];
};

/* Test fixture setup and teardown */
static int setup(void **state) {
    // Allocate test data
    struct test_data *data = malloc(sizeof(struct test_data));
    if (data == NULL) {
        return -1;
    }
    
    // Initialize test data
    memset(data, 0, sizeof(struct test_data));
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

/* Test putc and getc functionality */
static void test_putc(void **state) {
    int result;
    (void)state; // Not used here

    // Test putc with a character
    result = sys_putc('A');
    assert_int_equal(result, KORA_SUCCESS);
    
    // Test putc with a newline
    result = sys_putc('\n');
    assert_int_equal(result, KORA_SUCCESS);
}

/* Test file I/O functionality */
static void test_file_io(void **state) {
    struct test_data *data = *state;
    int result;
    
    /* Test sys_open with create */
    data->file_handle = sys_open(TEST_FILE, KORA_O_WRONLY | KORA_O_CREAT | KORA_O_TRUNC);
    assert_true(data->file_handle >= 0);
    
    /* Test sys_write */
    result = sys_write(data->file_handle, TEST_DATA, strlen(TEST_DATA));
    assert_int_equal(result, strlen(TEST_DATA));
    
    /* Test sys_close */
    result = sys_close(data->file_handle);
    assert_int_equal(result, KORA_SUCCESS);
    data->file_handle = -1;
    
    /* Test sys_open for reading */
    data->file_handle = sys_open(TEST_FILE, KORA_O_RDONLY);
    assert_true(data->file_handle >= 0);
    
    /* Test sys_read */
    memset(data->buffer, 0, BUFFER_SIZE);
    result = sys_read(data->file_handle, data->buffer, BUFFER_SIZE - 1);
    assert_true(result >= 0);
    assert_string_equal(data->buffer, TEST_DATA);
    
    /* Test sys_seek */
    result = sys_seek(data->file_handle, 0, KORA_SEEK_SET);
    assert_int_equal(result, 0);
    
    /* Read first 5 bytes after seek */
    memset(data->buffer, 0, BUFFER_SIZE);
    result = sys_read(data->file_handle, data->buffer, 5);
    assert_int_equal(result, 5);
    data->buffer[5] = '\0'; // Ensure null termination
    assert_string_equal(data->buffer, "Hello");
    
    /* Test sys_close again */
    result = sys_close(data->file_handle);
    assert_int_equal(result, KORA_SUCCESS);
    data->file_handle = -1;
}

/* Test ioctl functionality (with simple terminal check) */
static void test_ioctl(void **state) {
    int fd, result;
    (void)state; // Not used here

    struct winsize {
        unsigned short ws_row;
        unsigned short ws_col;
        unsigned short ws_xpixel;
        unsigned short ws_ypixel;
    } ws;
    
    /* Open stdin file descriptor */
    fd = 0; /* stdin */
    
    /* Try to get terminal window size using TIOCGWINSZ ioctl */
    result = sys_ioctl(fd, 0x5413 /* TIOCGWINSZ */, &ws);
    
    /* We don't fail the test if this fails, as it depends on the environment */
    if (result == KORA_SUCCESS) {
        printf("Terminal size: %d rows x %d columns\n", ws.ws_row, ws.ws_col);
    } else {
        printf("ioctl for window size failed (this is expected if not running in a terminal)\n");
    }
    
    /* This test is essentially a no-op for validation purposes */
    assert_true(1);
}

/* Main test suite */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_putc, setup, teardown),
        cmocka_unit_test_setup_teardown(test_file_io, setup, teardown),
        cmocka_unit_test_setup_teardown(test_ioctl, setup, teardown),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
} 
