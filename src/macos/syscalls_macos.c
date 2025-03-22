#include <internal/syscall_impl.h>

/**
 * macOS implementation of KoraOS system calls
 * Not yet implemented
 */

#if defined(KORA_PLATFORM_MACOS)
int macos_sys_putc(char c) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}

int macos_sys_getc(void) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}

int macos_sys_open(const char *path, int flags) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}

int macos_sys_close(int fd) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}

int macos_sys_read(int fd, void *buf, size_t count) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}

int macos_sys_write(int fd, const void *buf, size_t count) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}

long macos_sys_seek(int fd, long offset, int whence) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}

int macos_sys_ioctl(int fd, unsigned long request, void *arg) {
    /* TODO: Implement macOS version */
    return KORA_ERROR;
}
#endif 