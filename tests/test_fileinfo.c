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
#include <kora/syscalls.h>

// Test file paths
#define TEST_DIR "./test_fileinfo_dir"
#define TEST_FILE "./test_fileinfo_dir/test_file.txt"
#define TEST_SYMLINK "./test_fileinfo_dir/test_symlink"
#define NONEXISTENT_PATH "./nonexistent_path"

// Test file content
#define TEST_CONTENT "This is test content for file information testing.\n"

// Helper function to create test directory and files
static int setup_test_files(void)
{
    printf("Setting up test files...\n");
    
    // Remove existing files if they exist
    sys_rmdir(TEST_DIR);
    
    // Create test directory
    int ret = sys_mkdir(TEST_DIR);
    if (ret < 0) {
        printf("Error creating test directory: %d\n", ret);
        return ret;
    }
    
    // Create test file
    int fd = sys_open(TEST_FILE, O_WRONLY | O_CREAT);
    if (fd < 0) {
        printf("Error creating test file: %d\n", fd);
        return fd;
    }
    
    // Write content to test file
    ret = sys_write(fd, TEST_CONTENT, strlen(TEST_CONTENT));
    if (ret < 0) {
        printf("Error writing to test file: %d\n", ret);
        sys_close(fd);
        return ret;
    }
    
    sys_close(fd);
    
    // Create symbolic link
    ret = sys_symlink(TEST_FILE, TEST_SYMLINK);
    if (ret < 0) {
        printf("Error creating symbolic link: %d\n", ret);
        return ret;
    }
    
    return 0;
}

// Helper function to clean up test files
static void cleanup_test_files(void)
{
    printf("Cleaning up test files...\n");
    
    // Remove test files
    sys_unlink(TEST_SYMLINK);
    sys_unlink(TEST_FILE);
    sys_rmdir(TEST_DIR);
}

// Test sys_exists function
static int test_exists(void)
{
    printf("Testing sys_exists...\n");
    
    uint8_t type;
    int ret;
    
    // Test directory existence
    ret = sys_exists(TEST_DIR, &type);
    if (ret <= 0) {
        printf("Error: Directory should exist, got %d\n", ret);
        return -1;
    }
    
    if (type != KORA_FILE_TYPE_DIRECTORY) {
        printf("Error: Expected directory type, got %d\n", type);
        return -1;
    }
    
    // Test file existence
    ret = sys_exists(TEST_FILE, &type);
    if (ret <= 0) {
        printf("Error: File should exist, got %d\n", ret);
        return -1;
    }
    
    if (type != KORA_FILE_TYPE_REGULAR) {
        printf("Error: Expected regular file type, got %d\n", type);
        return -1;
    }
    
    // Test symlink existence
    ret = sys_exists(TEST_SYMLINK, &type);
    if (ret <= 0) {
        printf("Error: Symlink should exist, got %d\n", ret);
        return -1;
    }
    
    if (type != KORA_FILE_TYPE_SYMLINK) {
        printf("Error: Expected symlink type, got %d\n", type);
        return -1;
    }
    
    // Test non-existent path
    ret = sys_exists(NONEXISTENT_PATH, &type);
    if (ret != 0) {
        printf("Error: Non-existent path should return 0, got %d\n", ret);
        return -1;
    }
    
    printf("sys_exists test passed!\n");
    return 0;
}

// Test sys_get_file_info function
static int test_get_file_info(void)
{
    printf("Testing sys_get_file_info...\n");
    
    kora_file_info_t info;
    int ret;
    
    // Test getting info for a regular file
    ret = sys_get_file_info(TEST_FILE, &info);
    if (ret < 0) {
        printf("Error getting file info: %d\n", ret);
        return -1;
    }
    
    if (info.type != KORA_FILE_TYPE_REGULAR) {
        printf("Error: Expected regular file type, got %d\n", info.type);
        return -1;
    }
    
    if (info.size != strlen(TEST_CONTENT)) {
        printf("Error: Expected file size %ld, got %ld\n", 
               (long)strlen(TEST_CONTENT), (long)info.size);
        return -1;
    }
    
    printf("File '%s' info:\n", TEST_FILE);
    printf("  Type: %d\n", info.type);
    printf("  Size: %ld bytes\n", (long)info.size);
    printf("  Attributes: 0x%X\n", info.attributes);
    printf("  Create time: %ld\n", (long)info.creation_time);
    printf("  Modify time: %ld\n", (long)info.modified_time);
    printf("  Access time: %ld\n", (long)info.access_time);
    
    // Test getting info for a directory
    ret = sys_get_file_info(TEST_DIR, &info);
    if (ret < 0) {
        printf("Error getting directory info: %d\n", ret);
        return -1;
    }
    
    if (info.type != KORA_FILE_TYPE_DIRECTORY) {
        printf("Error: Expected directory type, got %d\n", info.type);
        return -1;
    }
    
    printf("Directory '%s' info:\n", TEST_DIR);
    printf("  Type: %d\n", info.type);
    printf("  Attributes: 0x%X\n", info.attributes);
    
    // Test getting info for a symlink
    ret = sys_get_file_info(TEST_SYMLINK, &info);
    if (ret < 0) {
        printf("Error getting symlink info: %d\n", ret);
        return -1;
    }
    
    printf("Symlink '%s' info:\n", TEST_SYMLINK);
    printf("  Type: %d\n", info.type);
    
    // Test getting info for a non-existent path
    ret = sys_get_file_info(NONEXISTENT_PATH, &info);
    if (ret >= 0) {
        printf("Error: Expected failure for non-existent path\n");
        return -1;
    }
    
    printf("sys_get_file_info test passed!\n");
    return 0;
}

// Test sys_get_fd_info function
static int test_get_fd_info(void)
{
    printf("Testing sys_get_fd_info...\n");
    
    kora_file_info_t info;
    int fd, ret;
    
    // Open test file
    fd = sys_open(TEST_FILE, O_RDONLY);
    if (fd < 0) {
        printf("Error opening test file: %d\n", fd);
        return -1;
    }
    
    // Get file info by descriptor
    ret = sys_get_fd_info(fd, &info);
    if (ret < 0) {
        printf("Error getting fd info: %d\n", ret);
        sys_close(fd);
        return -1;
    }
    
    if (info.type != KORA_FILE_TYPE_REGULAR) {
        printf("Error: Expected regular file type, got %d\n", info.type);
        sys_close(fd);
        return -1;
    }
    
    if (info.size != strlen(TEST_CONTENT)) {
        printf("Error: Expected file size %ld, got %ld\n", 
               (long)strlen(TEST_CONTENT), (long)info.size);
        sys_close(fd);
        return -1;
    }
    
    printf("File descriptor %d info:\n", fd);
    printf("  Type: %d\n", info.type);
    printf("  Size: %ld bytes\n", (long)info.size);
    printf("  Attributes: 0x%X\n", info.attributes);
    
    // Close test file
    sys_close(fd);
    
    // Test invalid file descriptor
    ret = sys_get_fd_info(-1, &info);
    if (ret >= 0) {
        printf("Error: Expected failure for invalid fd\n");
        return -1;
    }
    
    printf("sys_get_fd_info test passed!\n");
    return 0;
}

int main(void)
{
    printf("======== File Info System Call Tests ========\n");
    
    // Setup test files
    if (setup_test_files() < 0) {
        printf("Failed to set up test files\n");
        return 1;
    }
    
    // Run tests
    int failed = 0;
    
    if (test_exists() < 0) {
        printf("sys_exists test failed\n");
        failed = 1;
    }
    
    if (test_get_file_info() < 0) {
        printf("sys_get_file_info test failed\n");
        failed = 1;
    }
    
    if (test_get_fd_info() < 0) {
        printf("sys_get_fd_info test failed\n");
        failed = 1;
    }
    
    // Clean up test files
    cleanup_test_files();
    
    if (failed) {
        printf("Some tests failed\n");
        return 1;
    }
    
    printf("All file info tests passed!\n");
    return 0;
} 