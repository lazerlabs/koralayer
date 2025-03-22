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

#ifdef __cplusplus
}
#endif 