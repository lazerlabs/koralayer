/**
 * KoraOS System Call Implementation Details
 * 
 * Internal header for platform-specific syscall implementations
 */

#pragma once

#include <kora/syscalls.h>

/**
 * Platform detection macros
 */
#if defined(__linux__) || defined(__linux) || defined(linux)
    #define KORA_PLATFORM_LINUX 1
#elif defined(__APPLE__) || defined(__MACH__)
    #define KORA_PLATFORM_MACOS 1
#elif defined(_WIN32) || defined(_WIN64)
    #define KORA_PLATFORM_WINDOWS 1
#else
    #error "Unsupported platform"
#endif

/**
 * Internal implementation of system calls
 * These functions are platform-specific
 */

#if defined(KORA_PLATFORM_LINUX)
    int linux_sys_putc(char c);
    int linux_sys_getc(void);
    int linux_sys_open(const char *path, int flags);
    int linux_sys_close(int fd);
    int linux_sys_read(int fd, void *buf, size_t count);
    int linux_sys_write(int fd, const void *buf, size_t count);
    long linux_sys_seek(int fd, long offset, int whence);
    int linux_sys_ioctl(int fd, unsigned long request, void *arg);
    int linux_sys_mkdir(const char *path);
    int linux_sys_rmdir(const char *path);
    int linux_sys_opendir(const char *path);
    int linux_sys_readdir(int dir, kora_dirent_t *entry);
    int linux_sys_closedir(int dir);
    int linux_sys_symlink(const char *target, const char *linkpath);
    int linux_sys_readlink(const char *path, char *buf, size_t size);
    int linux_sys_unlink(const char *path);
    int linux_sys_rename(const char *oldpath, const char *newpath);
    void *linux_sys_brk(void *new_end);
    void *linux_sys_sbrk(ptrdiff_t delta);
    int linux_sys_get_file_info(const char *path, kora_file_info_t *info);
    int linux_sys_get_fd_info(int fd, kora_file_info_t *info);
    int linux_sys_exists(const char *path, uint8_t *type);
#elif defined(KORA_PLATFORM_MACOS)
    int macos_sys_putc(char c);
    int macos_sys_getc(void);
    int macos_sys_open(const char *path, int flags);
    int macos_sys_close(int fd);
    int macos_sys_read(int fd, void *buf, size_t count);
    int macos_sys_write(int fd, const void *buf, size_t count);
    long macos_sys_seek(int fd, long offset, int whence);
    int macos_sys_ioctl(int fd, unsigned long request, void *arg);
    int macos_sys_mkdir(const char *path);
    int macos_sys_rmdir(const char *path);
    int macos_sys_opendir(const char *path);
    int macos_sys_readdir(int dir, kora_dirent_t *entry);
    int macos_sys_closedir(int dir);
    int macos_sys_symlink(const char *target, const char *linkpath);
    int macos_sys_readlink(const char *path, char *buf, size_t size);
    int macos_sys_unlink(const char *path);
    int macos_sys_rename(const char *oldpath, const char *newpath);
    void *macos_sys_brk(void *new_end);
    void *macos_sys_sbrk(ptrdiff_t delta);
    int macos_sys_get_file_info(const char *path, kora_file_info_t *info);
    int macos_sys_get_fd_info(int fd, kora_file_info_t *info);
    int macos_sys_exists(const char *path, uint8_t *type);
#elif defined(KORA_PLATFORM_WINDOWS)
    int windows_sys_putc(char c);
    int windows_sys_getc(void);
    int windows_sys_open(const char *path, int flags);
    int windows_sys_close(int fd);
    int windows_sys_read(int fd, void *buf, size_t count);
    int windows_sys_write(int fd, const void *buf, size_t count);
    long windows_sys_seek(int fd, long offset, int whence);
    int windows_sys_ioctl(int fd, unsigned long request, void *arg);
    int windows_sys_mkdir(const char *path);
    int windows_sys_rmdir(const char *path);
    int windows_sys_opendir(const char *path);
    int windows_sys_readdir(int dir, kora_dirent_t *entry);
    int windows_sys_closedir(int dir);
    int windows_sys_symlink(const char *target, const char *linkpath);
    int windows_sys_readlink(const char *path, char *buf, size_t size);
    int windows_sys_unlink(const char *path);
    int windows_sys_rename(const char *oldpath, const char *newpath);
    void *windows_sys_brk(void *new_end);
    void *windows_sys_sbrk(ptrdiff_t delta);
    int windows_sys_get_file_info(const char *path, kora_file_info_t *info);
    int windows_sys_get_fd_info(int fd, kora_file_info_t *info);
    int windows_sys_exists(const char *path, uint8_t *type);
#endif 
