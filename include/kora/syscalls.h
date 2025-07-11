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
#include <semaphore.h> /* For sem_t */
#include <time.h>      /* For timespec */
#include <sys/time.h>  /* For timeval */
#include <signal.h>    /* For sighandler_t */

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
#define SYS_YIELD      29  /* Yield the processor */
#define SYS_GETPID     30  /* Get current process ID */
#define SYS_GETPPID    31  /* Get parent process ID */
#define SYS_SETPRIORITY 32 /* Set process scheduling priority */
#define SYS_PIPE       33  /* Create a pipe */
#define SYS_DUP        34  /* Duplicate a file descriptor */
#define SYS_DUP2       35  /* Duplicate to a specific descriptor */
#define SYS_SELECT     36  /* Wait for descriptor readiness */
#define SYS_SEM_WAIT   37  /* Wait on a semaphore */
#define SYS_SEM_POST   38  /* Post to a semaphore */
#define SYS_CLOCK_GETTIME 39 /* Get time from a clock */
#define SYS_GETTIMEOFDAY 40 /* Legacy time retrieval */
#define SYS_NANOSLEEP  41  /* Sleep for nanoseconds */
#define SYS_SLEEP      42  /* Sleep for seconds */
#define SYS_SETITIMER  43  /* Set interval timer */
#define SYS_STAT       44  /* Get file status by path */
#define SYS_FSTAT      45  /* Get file status by descriptor */
#define SYS_LSTAT      46  /* Get link status without following */
#define SYS_LINK       47  /* Create a hard link */
#define SYS_CHDIR      48  /* Change current directory */
#define SYS_GETCWD     49  /* Get current working directory */
#define SYS_UTIME      50  /* Update file timestamps */
#define SYS_SIGNAL     51  /* Install a signal handler */
#define SYS_KILL       52  /* Send a signal to a task */
#define SYS_SIGRETURN  53  /* Return from signal handler */
#define SYS_SYNC       54  /* Flush filesystem buffers */
#define SYS_REBOOT     55  /* Reboot or power off */
#define SYS_MOUNT      56  /* Mount a filesystem */

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
 * Simplified file status structure used by sys_stat family
 */
typedef struct {
    uint32_t mode;    /* File mode and type */
    uint64_t size;    /* Size in bytes */
    uint64_t mtime;   /* Modification time (UNIX timestamp) */
    uint32_t uid;     /* Owning user ID */
    uint32_t gid;     /* Owning group ID */
} kora_stat_t;

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
 * Get file status information by path
 */
int sys_stat(const char *path, kora_stat_t *st);

/**
 * Get file status information by descriptor
 */
int sys_fstat(int fd, kora_stat_t *st);

/**
 * Get file status without following symlinks
 */
int sys_lstat(const char *path, kora_stat_t *st);

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

/** Create a hard link to a file */
int sys_link(const char *existing, const char *newpath);

/** Change the current working directory */
int sys_chdir(const char *path);

/** Retrieve the current working directory into buf */
int sys_getcwd(char *buf, size_t size);

/** Update access and modification times */
int sys_utime(const char *path, uint64_t mtime);

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

/**
 * Yield the processor to another runnable task.
 *
 * @return 0 on success, -1 on failure
 */
int sys_yield(void);

/** Get the current process ID */
pid_t sys_getpid(void);

/** Get the parent process ID */
pid_t sys_getppid(void);

/** Set scheduling priority for a process */
int sys_setpriority(pid_t pid, int prio);

/** Create an anonymous pipe */
int sys_pipe(int fds[2]);

/** Duplicate a file descriptor */
int sys_dup(int oldfd);

/** Duplicate to a specific file descriptor */
int sys_dup2(int oldfd, int newfd);

/** Wait for descriptor readiness */
int sys_select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tmo);

/** Wait on a semaphore */
int sys_sem_wait(sem_t *sem);

/** Post to a semaphore */
int sys_sem_post(sem_t *sem);

/** Get time from a specific clock */
int sys_clock_gettime(clockid_t id, struct timespec *tp);

/** Get the current time of day */
int sys_gettimeofday(struct timeval *tv, void *tz);

/** Sleep for a number of nanoseconds */
int sys_nanosleep(const struct timespec *req, struct timespec *rem);

/** Sleep for a whole number of seconds */
unsigned sys_sleep(unsigned seconds);

/** Set an interval timer */
int sys_setitimer(int which, const struct itimerval *new, struct itimerval *old);

/** Install a signal handler */
sighandler_t sys_signal(int signum, sighandler_t handler);

/** Send a signal to a process or group */
int sys_kill(pid_t pid, int signum);

/** Return from a signal handler */
int sys_sigreturn(void);

/** Flush filesystem buffers to disk */
int sys_sync(void);

/** Reboot or power off */
int sys_reboot(int cmd);

/** Mount a filesystem */
int sys_mount(const char *src, const char *tgt, const char *type,
              unsigned flags, const void *data);

#ifdef __cplusplus
}
#endif 
