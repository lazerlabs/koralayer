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
#endif
