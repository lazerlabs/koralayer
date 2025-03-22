#include <kora/syscalls.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_FILE "test_file.txt"
#define TEST_DATA "Hello, KoraOS!\n"
#define BUFFER_SIZE 128

/* Test helper functions */
void print_result(const char *test_name, int passed) {
    printf("%-30s: %s\n", test_name, passed ? "PASSED" : "FAILED");
}

int test_putc_getc() {
    int result;
    int passed = 1;
    
    printf("\nTesting sys_putc and sys_getc...\n");
    printf("Please type a character: ");
    fflush(stdout);
    
    /* Get a character from stdin using sys_getc */
    int c = sys_getc();
    if (c < 0) {
        printf("sys_getc failed\n");
        return 0;
    }
    
    /* Echo it back using sys_putc */
    printf("\nYou typed: ");
    result = sys_putc((char)c);
    sys_putc('\n');
    
    if (result != KORA_SUCCESS) {
        printf("sys_putc failed\n");
        passed = 0;
    }
    
    return passed;
}

int test_file_io() {
    int fd, result;
    char buffer[BUFFER_SIZE];
    int passed = 1;
    
    printf("\nTesting file I/O syscalls...\n");
    
    /* Test sys_open with create */
    fd = sys_open(TEST_FILE, KORA_O_WRONLY | KORA_O_CREAT | KORA_O_TRUNC);
    if (fd < 0) {
        printf("sys_open for writing failed\n");
        return 0;
    }
    
    /* Test sys_write */
    result = sys_write(fd, TEST_DATA, strlen(TEST_DATA));
    if (result != strlen(TEST_DATA)) {
        printf("sys_write failed: wrote %d bytes, expected %zu\n", 
               result, strlen(TEST_DATA));
        passed = 0;
    }
    
    /* Test sys_close */
    result = sys_close(fd);
    if (result != KORA_SUCCESS) {
        printf("sys_close failed\n");
        passed = 0;
    }
    
    /* Test sys_open for reading */
    fd = sys_open(TEST_FILE, KORA_O_RDONLY);
    if (fd < 0) {
        printf("sys_open for reading failed\n");
        return 0;
    }
    
    /* Test sys_read */
    memset(buffer, 0, BUFFER_SIZE);
    result = sys_read(fd, buffer, BUFFER_SIZE - 1);
    if (result < 0) {
        printf("sys_read failed\n");
        passed = 0;
    } else if (strcmp(buffer, TEST_DATA) != 0) {
        printf("Read data mismatch\n");
        printf("Expected: '%s'\n", TEST_DATA);
        printf("Got:      '%s'\n", buffer);
        passed = 0;
    }
    
    /* Test sys_seek */
    result = sys_seek(fd, 0, KORA_SEEK_SET);
    if (result != 0) {
        printf("sys_seek to beginning failed\n");
        passed = 0;
    }
    
    /* Read first 5 bytes after seek */
    memset(buffer, 0, BUFFER_SIZE);
    result = sys_read(fd, buffer, 5);
    if (result != 5 || strncmp(buffer, "Hello", 5) != 0) {
        printf("sys_read after seek failed\n");
        passed = 0;
    }
    
    /* Test sys_close again */
    result = sys_close(fd);
    if (result != KORA_SUCCESS) {
        printf("Second sys_close failed\n");
        passed = 0;
    }
    
    return passed;
}

int test_ioctl() {
    /* Test a simple terminal IOCTL to get window size */
    int fd, result;
    struct winsize {
        unsigned short ws_row;
        unsigned short ws_col;
        unsigned short ws_xpixel;
        unsigned short ws_ypixel;
    } ws;
    int passed = 1;
    
    printf("\nTesting sys_ioctl...\n");
    
    /* Open stdin file descriptor */
    fd = 0; /* stdin */
    
    /* Try to get terminal window size using TIOCGWINSZ ioctl */
    /* Note: This is just a simple test, might not work in all environments */
    result = sys_ioctl(fd, 0x5413 /* TIOCGWINSZ */, &ws);
    
    /* We don't require this to pass as it depends on whether stdin is a terminal */
    if (result == KORA_SUCCESS) {
        printf("Terminal size: %d rows x %d columns\n", ws.ws_row, ws.ws_col);
    } else {
        printf("ioctl for window size failed (this is expected if not running in a terminal)\n");
    }
    
    /* Always return success as this test is informational */
    return passed;
}

int main() {
    int tests_passed = 0;
    int tests_total = 0;
    int test_result;
    
    printf("KoraOS I/O System Call Tests\n");
    printf("============================\n");
    
    /* Test 1: putc/getc */
    tests_total++;
    test_result = test_putc_getc();
    print_result("putc/getc test", test_result);
    tests_passed += test_result;
    
    /* Test 2: File I/O */
    tests_total++;
    test_result = test_file_io();
    print_result("File I/O test", test_result);
    tests_passed += test_result;
    
    /* Test 3: ioctl */
    tests_total++;
    test_result = test_ioctl();
    print_result("ioctl test", test_result);
    tests_passed += test_result;
    
    /* Print summary */
    printf("\nSummary: %d/%d tests passed\n", tests_passed, tests_total);
    
    return (tests_passed == tests_total) ? 0 : 1;
} 