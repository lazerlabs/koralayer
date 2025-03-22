#include <internal/syscall_impl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

/**
 * Linux implementation of KoraOS system calls
 * Uses glibc rather than direct syscalls
 */

int linux_sys_putc(char c) {
    int result = putchar(c);
    if (result == EOF) {
        return KORA_ERROR;
    }
    return KORA_SUCCESS;
}

int linux_sys_getc(void) {
    int c = getchar();
    if (c == EOF) {
        return KORA_EOF;
    }
    return c;
}

/**
 * Convert Kora open flags to Linux open flags
 */
static int convert_open_flags(int kora_flags) {
    int linux_flags = 0;
    
    /* Access mode */
    if ((kora_flags & KORA_O_RDWR) == KORA_O_RDWR) {
        linux_flags |= O_RDWR;
    } else if (kora_flags & KORA_O_WRONLY) {
        linux_flags |= O_WRONLY;
    } else {
        linux_flags |= O_RDONLY;
    }
    
    /* Creation flags */
    if (kora_flags & KORA_O_CREAT) {
        linux_flags |= O_CREAT;
    }
    if (kora_flags & KORA_O_TRUNC) {
        linux_flags |= O_TRUNC;
    }
    if (kora_flags & KORA_O_APPEND) {
        linux_flags |= O_APPEND;
    }
    
    return linux_flags;
}

int linux_sys_open(const char *path, int flags) {
    int linux_flags = convert_open_flags(flags);
    int fd;
    
    /* Handle file creation with default permissions */
    if (flags & KORA_O_CREAT) {
        fd = open(path, linux_flags, 0644);
    } else {
        fd = open(path, linux_flags);
    }
    
    if (fd < 0) {
        return KORA_ERROR;
    }
    
    return fd;
}

int linux_sys_close(int fd) {
    int result = close(fd);
    if (result < 0) {
        return KORA_ERROR;
    }
    return KORA_SUCCESS;
}

int linux_sys_read(int fd, void *buf, size_t count) {
    ssize_t result = read(fd, buf, count);
    
    if (result < 0) {
        return KORA_ERROR;
    }
    
    if (result == 0) {
        return KORA_EOF;
    }
    
    return (int)result;
}

int linux_sys_write(int fd, const void *buf, size_t count) {
    ssize_t result = write(fd, buf, count);
    
    if (result < 0) {
        return KORA_ERROR;
    }
    
    return (int)result;
}

long linux_sys_seek(int fd, long offset, int whence) {
    int linux_whence;
    
    /* Convert Kora seek mode to Linux seek mode */
    switch (whence) {
        case KORA_SEEK_SET:
            linux_whence = SEEK_SET;
            break;
        case KORA_SEEK_CUR:
            linux_whence = SEEK_CUR;
            break;
        case KORA_SEEK_END:
            linux_whence = SEEK_END;
            break;
        default:
            return KORA_ERROR;
    }
    
    off_t result = lseek(fd, offset, linux_whence);
    
    if (result < 0) {
        return KORA_ERROR;
    }
    
    return (long)result;
}

int linux_sys_ioctl(int fd, unsigned long request, void *arg) {
    int result = ioctl(fd, request, arg);
    
    if (result < 0) {
        return KORA_ERROR;
    }
    
    return KORA_SUCCESS;
} 

