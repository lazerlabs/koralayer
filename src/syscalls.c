#include <kora/syscalls.h>
#include <internal/syscall_impl.h>

/**
 * Generic syscall implementations that dispatch to platform-specific functions
 */

int sys_putc(char c) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_putc(c);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_putc(c);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_putc(c);
#endif
}

int sys_getc(void) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_getc();
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_getc();
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_getc();
#endif
}

int sys_open(const char *path, int flags) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_open(path, flags);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_open(path, flags);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_open(path, flags);
#endif
}

int sys_close(int fd) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_close(fd);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_close(fd);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_close(fd);
#endif
}

int sys_read(int fd, void *buf, size_t count) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_read(fd, buf, count);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_read(fd, buf, count);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_read(fd, buf, count);
#endif
}

int sys_write(int fd, const void *buf, size_t count) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_write(fd, buf, count);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_write(fd, buf, count);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_write(fd, buf, count);
#endif
}

long sys_seek(int fd, long offset, int whence) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_seek(fd, offset, whence);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_seek(fd, offset, whence);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_seek(fd, offset, whence);
#endif
}

int sys_ioctl(int fd, unsigned long request, void *arg) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_ioctl(fd, request, arg);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_ioctl(fd, request, arg);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_ioctl(fd, request, arg);
#endif
} 

int sys_mkdir(const char *path) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_mkdir(path);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_mkdir(path);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_mkdir(path);
#endif
}

int sys_rmdir(const char *path) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_rmdir(path);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_rmdir(path);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_rmdir(path);
#endif
}

int sys_opendir(const char *path) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_opendir(path);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_opendir(path);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_opendir(path);
#endif
}

int sys_readdir(int dir, kora_dirent_t *entry) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_readdir(dir, entry);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_readdir(dir, entry);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_readdir(dir, entry);
#endif
}

int sys_closedir(int dir) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_closedir(dir);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_closedir(dir);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_closedir(dir);
#endif
}

int sys_symlink(const char *target, const char *linkpath) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_symlink(target, linkpath);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_symlink(target, linkpath);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_symlink(target, linkpath);
#endif
}

int sys_readlink(const char *path, char *buf, size_t size) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_readlink(path, buf, size);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_readlink(path, buf, size);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_readlink(path, buf, size);
#endif
} 

int sys_get_file_info(const char *path, kora_file_info_t *info) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_get_file_info(path, info);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_get_file_info(path, info);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_get_file_info(path, info);
#endif
}

int sys_get_fd_info(int fd, kora_file_info_t *info) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_get_fd_info(fd, info);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_get_fd_info(fd, info);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_get_fd_info(fd, info);
#endif
}

int sys_exists(const char *path, uint8_t *type) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_exists(path, type);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_exists(path, type);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_exists(path, type);
#endif
}


int sys_unlink(const char *path) {
#if defined(KORA_PLATFORM_LINUX)
    return linux_sys_unlink(path);
#elif defined(KORA_PLATFORM_MACOS)
    return macos_sys_unlink(path);
#elif defined(KORA_PLATFORM_WINDOWS)
    return windows_sys_unlink(path);
#endif
}
