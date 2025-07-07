#include <internal/syscall_impl.h>

/**
 * Windows implementation of KoraOS system calls
 * Not yet implemented
 */

#if defined(KORA_PLATFORM_WINDOWS)
int windows_sys_putc(char c) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

int windows_sys_getc(void) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

int windows_sys_open(const char *path, int flags) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

int windows_sys_close(int fd) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

int windows_sys_read(int fd, void *buf, size_t count) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

int windows_sys_write(int fd, const void *buf, size_t count) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

long windows_sys_seek(int fd, long offset, int whence) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

int windows_sys_ioctl(int fd, unsigned long request, void *arg) {
    /* TODO: Implement Windows version */
    return KORA_ERROR;
}

void *windows_sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off) {
    /* TODO: Implement Windows version */
    (void)addr; (void)len; (void)prot; (void)flags; (void)fd; (void)off;
    return (void *)-1;
}

int windows_sys_munmap(void *addr, size_t len) {
    /* TODO: Implement Windows version */
    (void)addr; (void)len;
    return -1;
}

int windows_sys_mprotect(void *addr, size_t len, int prot) {
    /* TODO: Implement Windows version */
    (void)addr; (void)len; (void)prot;
    return -1;
}

#endif // KORA_PLATFORM_WINDOWS
