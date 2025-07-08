/**
 * KoraOS System Call Interface
 * 
 * This header defines the system calls provided by KoraOS
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>  /* For size_t */
#include <stdint.h>  /* For uint32_t, uint64_t */
#include <sys/types.h> /* For pid_t */

/**
 * System call numbers
 */
#define SYS_PUTC       1   /* Write a character to standard output */
#define SYS_GETC       2   /* Read a character from standard input */
#define SYS_OPEN       3   /* Open a file and return a file descriptor */
#define SYS_CLOSE      4   /* Close a file descriptor */
#define SYS_READ       5   /* Read from a file descriptor into a buffer */
#define SYS_WRITE      6   /* Write a buffer to a file descriptor */
#define SYS_SEEK       7   /* Reposition read/write file offset */
#define SYS_IOCTL      8   /* Device-specific control operations */
#define SYS_MKDIR      9   /* Create a directory */
#define SYS_RMDIR      10  /* Remove a directory */
#define SYS_OPENDIR    11  /* Open a directory for reading */
#define SYS_READDIR    12  /* Read a directory entry */
#define SYS_CLOSEDIR   13  /* Close a directory */
#define SYS_SYMLINK    14  /* Create a symbolic link */
#define SYS_READLINK   15  /* Read the target of a symbolic link */
#define SYS_UNLINK     16  /* Delete a name from the filesystem */
#define SYS_GET_FILE_INFO 17 /* Get file information by path */
#define SYS_GET_FD_INFO   18 /* Get file information by descriptor */
#define SYS_EXISTS     19  /* Check if a path exists */
#define SYS_RENAME     20  /* Rename a file or directory */
#define SYS_BRK        21  /* Set program break */
#define SYS_SBRK       22  /* Change program break by delta */
#define SYS_MMAP       23  /* Map memory pages */
#define SYS_MUNMAP     24  /* Unmap memory pages */
#define SYS_MPROTECT   25  /* Change memory protection */
#define SYS_SPAWN      26  /* Spawn a new process */
#define SYS_EXIT       27  /* Terminate the calling task */
#define SYS_WAIT       28  /* Wait for a child process */

/**
 * File open flags
 */
#define KORA_O_RDONLY  0x0001  /* Open for reading only */
#define KORA_O_WRONLY  0x0002  /* Open for writing only */
#define KORA_O_RDWR    (KORA_O_RDONLY | KORA_O_WRONLY)  /* Open for reading and writing */
#define KORA_O_CREAT   0x0100  /* Create file if it does not exist */
#define KORA_O_TRUNC   0x0200  /* Truncate file to zero length */
#define KORA_O_APPEND  0x0400  /* Append to the file */

/**
 * Seek modes
 */
#define KORA_SEEK_SET  0     /* Set position to offset */
#define KORA_SEEK_CUR  1     /* Set position to current location plus offset */
#define KORA_SEEK_END  2     /* Set position to EOF plus offset */

/**
 * Status/error codes
 */
#define KORA_SUCCESS    0
#define KORA_ERROR     -1
#define KORA_EOF       -2

/**
 * Directory entry structure
 */
typedef struct kora_dirent {
    char name[256];        /* Filename */
    unsigned char type;    /* File type */
} kora_dirent_t;

/**
 * File types for kora_dirent.type
 */
#define KORA_DT_UNKNOWN  0  /* Unknown file type */
#define KORA_DT_REG      1  /* Regular file */
#define KORA_DT_DIR      2  /* Directory */
#define KORA_DT_SYMLINK  3  /* Symbolic link */

/**
 * File types for kora_file_info_t.type
 */
#define KORA_FILE_TYPE_UNKNOWN    0  /* Unknown file type */
#define KORA_FILE_TYPE_REGULAR    1  /* Regular file */
#define KORA_FILE_TYPE_DIRECTORY  2  /* Directory */
#define KORA_FILE_TYPE_SYMLINK    3  /* Symbolic link */
#define KORA_FILE_TYPE_OTHER      4  /* Other file type */

/**
 * File attributes for kora_file_info_t.attributes
 */
#define KORA_FILE_ATTR_READONLY   0x01  /* Read-only file */
#define KORA_FILE_ATTR_HIDDEN     0x02  /* Hidden file */
#define KORA_FILE_ATTR_SYSTEM     0x04  /* System file */
#define KORA_FILE_ATTR_ARCHIVE    0x08  /* Archive attribute */

/**
 * File information structure
 */
typedef struct {
    uint8_t type;                /* File type (KORA_FILE_TYPE_*) */
    uint8_t attributes;          /* File attributes (KORA_FILE_ATTR_*) */
    uint32_t starting_cluster;   /* First cluster of file in the filesystem */
    uint64_t size;               /* File size in bytes */
    uint64_t creation_time;      /* File creation time (UNIX timestamp) */
    uint64_t modified_time;      /* Last modification time (UNIX timestamp) */
    uint64_t access_time;        /* Last access time (UNIX timestamp) */
} kora_file_info_t;

/**
 * File attribute flags
 */
#define KORA_ATTR_READONLY  0x01  /* Read-only file */
#define KORA_ATTR_HIDDEN    0x02  /* Hidden file */
#define KORA_ATTR_SYSTEM    0x04  /* System file */
#define KORA_ATTR_ARCHIVE   0x08  /* Archive flag */

/**
 * Function prototypes for KoraOS system calls
 */

/**
 * Write a single character to standard output
 * 
 * @param c The character to write
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_putc(char c);

/**
 * Read a single character from standard input
 * 
 * @return The character read, or KORA_EOF on end of file, or KORA_ERROR on error
 */
int sys_getc(void);

/**
 * Open a file and return a file descriptor
 * 
 * @param path Path to the file
 * @param flags Flags controlling how the file is opened
 * @return File descriptor on success, KORA_ERROR on failure
 */
int sys_open(const char *path, int flags);

/**
 * Close a file descriptor
 * 
 * @param fd File descriptor to close
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_close(int fd);

/**
 * Read from a file descriptor into a buffer
 * 
 * @param fd File descriptor to read from
 * @param buf Buffer to read into
 * @param count Number of bytes to read
 * @return Number of bytes read on success, KORA_EOF on end of file, KORA_ERROR on error
 */
int sys_read(int fd, void *buf, size_t count);

/**
 * Write a buffer to a file descriptor
 * 
 * @param fd File descriptor to write to
 * @param buf Buffer to write from
 * @param count Number of bytes to write
 * @return Number of bytes written on success, KORA_ERROR on error
 */
int sys_write(int fd, const void *buf, size_t count);

/**
 * Reposition read/write file offset
 * 
 * @param fd File descriptor
 * @param offset Offset from origin
 * @param whence Position origin (KORA_SEEK_SET, KORA_SEEK_CUR, KORA_SEEK_END)
 * @return New offset from beginning of file on success, KORA_ERROR on error
 */
long sys_seek(int fd, long offset, int whence);

/**
 * Device-specific control operations
 * 
 * @param fd File descriptor for device
 * @param request Device-specific request code
 * @param arg Optional argument depending on request
 * @return KORA_SUCCESS on success, KORA_ERROR on error
 */
int sys_ioctl(int fd, unsigned long request, void *arg);

/**
 * Create a directory
 * 
 * @param path Path to the directory to create
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_mkdir(const char *path);

/**
 * Remove a directory
 * 
 * @param path Path to the directory to remove
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_rmdir(const char *path);

/**
 * Open a directory for reading
 * 
 * @param path Path to the directory
 * @return Directory handle on success, KORA_ERROR on failure
 */
int sys_opendir(const char *path);

/**
 * Read a directory entry
 * 
 * @param dir Directory handle
 * @param entry Pointer to kora_dirent_t structure to fill
 * @return 1 if an entry was read, 0 if end of directory, KORA_ERROR on failure
 */
int sys_readdir(int dir, kora_dirent_t *entry);

/**
 * Close a directory
 * 
 * @param dir Directory handle
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_closedir(int dir);

/**
 * Create a symbolic link
 * 
 * @param target Target path that the link will point to
 * @param linkpath Path where the symbolic link will be created
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_symlink(const char *target, const char *linkpath);

/**
 * Read the target of a symbolic link
 * 
 * @param path Path to the symbolic link
 * @param buf Buffer to store the target path
 * @param size Size of the buffer
 * @return Length of the target path on success, KORA_ERROR on failure
 */
int sys_readlink(const char *path, char *buf, size_t size);

/**
 * Get file information by path
 * 
 * @param path Path to the file
 * @param info Pointer to kora_file_info_t structure to fill
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_get_file_info(const char *path, kora_file_info_t *info);

/**
 * Get file information by descriptor
 * 
 * @param fd File descriptor
 * @param info Pointer to kora_file_info_t structure to fill
 * @return KORA_SUCCESS on success, KORA_ERROR on failure
 */
int sys_get_fd_info(int fd, kora_file_info_t *info);

/**
 * Check if a path exists
 * 
 * @param path Path to check
 * @param type Pointer to a uint8_t to store the file type (KORA_DT_* values), can be NULL
 * @return KORA_SUCCESS if path exists, KORA_ERROR if path does not exist or error
 */
int sys_exists(const char *path, uint8_t *type);

/**
 * Delete a name from the filesystem
 * @param path Path to the file
 * @return 0 on success, negative error code on failure
 */
int sys_unlink(const char *path);

/**
 * Rename a file or directory
 * @param oldpath Existing path
 * @param newpath New path name
 * @return 0 on success, negative error code on failure
 */
int sys_rename(const char *oldpath, const char *newpath);

/**
 * Set the program break to a specific address
 * @param new_end New end of the data segment
 * @return New program break on success, (void*)-1 on failure
 */
void *sys_brk(void *new_end);

/**
 * Adjust the program break by a delta
 * @param delta Number of bytes to add to the break (can be negative)
 * @return Previous program break on success, (void*)-1 on failure
 */
void *sys_sbrk(ptrdiff_t delta);

/**
 * Map anonymous or file-backed memory into the process address space
 * @param addr Desired address or NULL
 * @param len Length of the mapping in bytes
 * @param prot Protection flags (e.g., PROT_READ, PROT_WRITE)
 * @param flags Mapping flags (e.g., MAP_PRIVATE, MAP_ANONYMOUS)
 * @param fd File descriptor if mapping a file, or -1 for anonymous
 * @param off Offset in the file
 * @return Pointer to mapped region on success, (void*)-1 on failure
 */
void *sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);

/**
 * Unmap a previously mapped memory region
 * @param addr Starting address of the mapping
 * @param len Length of the mapping
 * @return 0 on success, -1 on failure
 */
int sys_munmap(void *addr, size_t len);

/**
 * Change the protection of a memory region
 * @param addr Start address
 * @param len Length of region
 * @param prot New protection flags
 * @return 0 on success, -1 on failure
 */
int sys_mprotect(void *addr, size_t len, int prot);

/**
 * Spawn a new process executing the program at `path`.
 * The child inherits file descriptors and environment.
 *
 * @param path Path to executable
 * @param argv NULL-terminated argument vector
 * @param envp NULL-terminated environment vector, or NULL to inherit
 * @return Child PID on success, -1 on failure
 */
pid_t sys_spawn(const char *path, char *const argv[], char *const envp[]);

/**
 * Terminate the calling task with the given status.
 * This function does not return.
 */
void sys_exit(int status) __attribute__((noreturn));

/**
 * Wait for a child process to exit.
 *
 * @param pid PID to wait for or -1 for any child
 * @param status Pointer to store exit status
 * @param options Options passed to waitpid
 * @return PID of the exited child or -1 on error
 */
pid_t sys_wait(pid_t pid, int *status, int options);

#ifdef __cplusplus
}
#endif 
