/**
 * Directory operations test for KoraLayer
 */

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

/**
 * Test mkdir and rmdir functionality
 */
void test_mkdir_rmdir(void) {
    int result;

    printf("Testing mkdir and rmdir...\n");

    // Create test directory
    result = sys_mkdir(TEST_DIR);
    if (result != KORA_SUCCESS) {
        printf("FAIL: sys_mkdir(%s) returned %d (errno: %d, %s)\n",
            TEST_DIR, result, errno, strerror(errno));
        exit(1);
    }
    printf("SUCCESS: Created directory %s\n", TEST_DIR);

    // Create subdirectory
    result = sys_mkdir(TEST_SUBDIR);
    if (result != KORA_SUCCESS) {
        printf("FAIL: sys_mkdir(%s) returned %d\n", TEST_SUBDIR, result);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    printf("SUCCESS: Created subdirectory %s\n", TEST_SUBDIR);

    // Remove subdirectory
    result = sys_rmdir(TEST_SUBDIR);
    if (result != KORA_SUCCESS) {
        printf("FAIL: sys_rmdir(%s) returned %d\n", TEST_SUBDIR, result);
        sys_rmdir(TEST_SUBDIR);  // Try again
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    printf("SUCCESS: Removed subdirectory %s\n", TEST_SUBDIR);

    // Don't remove the main test directory yet as we'll use it for other tests
}

/**
 * Test directory reading functionality
 */
void test_directory_reading(void) {
    int dir;
    int result;
    kora_dirent_t entry;
    int found_dot = 0;
    int found_dotdot = 0;
    int found_test_file = 0;

    printf("Testing directory reading...\n");

    // Create a test file
    int fd = sys_open(TEST_FILE, KORA_O_CREAT | KORA_O_WRONLY);
    if (fd < 0) {
        printf("FAIL: Could not create test file %s (errno: %d, %s)\n", 
               TEST_FILE, errno, strerror(errno));
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    
    sys_write(fd, "test", 4);
    sys_close(fd);
    printf("Created test file for directory reading test\n");

    // Open directory
    dir = sys_opendir(TEST_DIR);
    if (dir < 0) {
        printf("FAIL: sys_opendir(%s) returned %d (errno: %d, %s)\n", 
               TEST_DIR, dir, errno, strerror(errno));
        unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    printf("SUCCESS: Opened directory %s\n", TEST_DIR);

    // Read directory entries
    while ((result = sys_readdir(dir, &entry)) > 0) {
        printf("Found directory entry: %s (type: %d)\n", entry.name, entry.type);
        
        if (strcmp(entry.name, ".") == 0) {
            found_dot = 1;
            if (entry.type != KORA_DT_DIR) {
                printf("FAIL: . is not marked as a directory\n");
            }
        }
        else if (strcmp(entry.name, "..") == 0) {
            found_dotdot = 1;
            if (entry.type != KORA_DT_DIR) {
                printf("FAIL: .. is not marked as a directory\n");
            }
        }
        else if (strcmp(entry.name, "test_file") == 0) {
            found_test_file = 1;
            if (entry.type != KORA_DT_REG) {
                printf("FAIL: test_file is not marked as a regular file\n");
            }
        }
    }
    
    if (result < 0) {
        printf("FAIL: sys_readdir returned error %d (errno: %d, %s)\n", 
               result, errno, strerror(errno));
        sys_closedir(dir);
        unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }

    // Check that we found all expected entries
    if (!found_dot || !found_dotdot || !found_test_file) {
        printf("FAIL: Did not find all expected directory entries\n");
        printf("  . found: %d\n", found_dot);
        printf("  .. found: %d\n", found_dotdot);
        printf("  test_file found: %d\n", found_test_file);
        sys_closedir(dir);
        unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    printf("SUCCESS: Found all expected directory entries\n");

    // Close directory
    result = sys_closedir(dir);
    if (result != KORA_SUCCESS) {
        printf("FAIL: sys_closedir returned %d (errno: %d, %s)\n", 
               result, errno, strerror(errno));
        unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    printf("SUCCESS: Closed directory\n");
    
    // Clean up test file
    if (sys_rmdir(TEST_FILE) == KORA_SUCCESS) {
        printf("FAIL: sys_rmdir succeeded on a file!\n");
    } else {
        printf("SUCCESS: sys_rmdir correctly failed on a non-directory\n");
    }
    
    // Remove the test file
    if (unlink(TEST_FILE) != 0) {
        printf("Warning: Failed to remove test file: %s\n", strerror(errno));
    }
}

/**
 * Test symlink and readlink functionality
 */
void test_symlink(void) {
    int result;
    char buf[256];

    printf("Testing symbolic links...\n");

    // Create a test file
    int fd = sys_open(TEST_FILE, KORA_O_CREAT | KORA_O_WRONLY);
    if (fd < 0) {
        printf("FAIL: Could not create test file %s\n", TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    sys_write(fd, "test", 4);
    sys_close(fd);
    printf("Created test file for symlink test\n");

    // Create symlink
    result = sys_symlink(TEST_FILE, TEST_SYMLINK);
    if (result != KORA_SUCCESS) {
        printf("FAIL: sys_symlink returned %d\n", result);
        unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    printf("SUCCESS: Created symlink %s -> %s\n", TEST_SYMLINK, TEST_FILE);

    // Read symlink
    result = sys_readlink(TEST_SYMLINK, buf, sizeof(buf));
    if (result < 0) {
        printf("FAIL: sys_readlink returned %d\n", result);
        unlink(TEST_SYMLINK);
        unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    
    printf("Symlink target: %s\n", buf);
    
    if (strcmp(buf, TEST_FILE) != 0) {
        printf("FAIL: Symlink target does not match\n");
        printf("  Expected: %s\n", TEST_FILE);
        printf("  Got: %s\n", buf);
        unlink(TEST_SYMLINK);
        unlink(TEST_FILE);
        sys_rmdir(TEST_DIR);
        exit(1);
    }
    printf("SUCCESS: Symlink target matches\n");

    // Clean up
    if (unlink(TEST_SYMLINK) != 0) {
        printf("Warning: Failed to remove symlink: %s\n", strerror(errno));
    }
    if (unlink(TEST_FILE) != 0) {
        printf("Warning: Failed to remove test file: %s\n", strerror(errno));
    }
}

/**
 * Main test function
 */
int main(void) {
    printf("=== KoraLayer Directory Operations Test ===\n");

    // Clean up any left-over test directories from previous runs
    rmdir(TEST_SUBDIR);  // Ignore errors
    unlink(TEST_SYMLINK);  // Ignore errors
    unlink(TEST_FILE);  // Ignore errors
    rmdir(TEST_DIR);  // Ignore errors

    test_mkdir_rmdir();
    test_directory_reading();
    test_symlink();

    // Final cleanup
    sys_rmdir(TEST_DIR);

    printf("=== All directory tests passed! ===\n");
    return 0;
} 