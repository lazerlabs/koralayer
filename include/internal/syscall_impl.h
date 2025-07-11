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
    void *linux_sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
    int linux_sys_munmap(void *addr, size_t len);
    int linux_sys_mprotect(void *addr, size_t len, int prot);
    int linux_sys_yield(void);
    pid_t linux_sys_getpid(void);
    pid_t linux_sys_getppid(void);
    int linux_sys_setpriority(pid_t pid, int prio);
    int linux_sys_pipe(int fds[2]);
    int linux_sys_dup(int oldfd);
    int linux_sys_dup2(int oldfd, int newfd);
    int linux_sys_select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tmo);
    int linux_sys_sem_wait(sem_t *sem);
    int linux_sys_sem_post(sem_t *sem);
    int linux_sys_clock_gettime(clockid_t id, struct timespec *tp);
    int linux_sys_gettimeofday(struct timeval *tv, void *tz);
    int linux_sys_nanosleep(const struct timespec *req, struct timespec *rem);
    unsigned linux_sys_sleep(unsigned seconds);
    int linux_sys_setitimer(int which, const struct itimerval *new, struct itimerval *old);
    int linux_sys_get_file_info(const char *path, kora_file_info_t *info);
    int linux_sys_get_fd_info(int fd, kora_file_info_t *info);
    int linux_sys_stat(const char *path, kora_stat_t *st);
    int linux_sys_fstat(int fd, kora_stat_t *st);
    int linux_sys_lstat(const char *path, kora_stat_t *st);
    int linux_sys_link(const char *existing, const char *newpath);
    int linux_sys_chdir(const char *path);
    int linux_sys_getcwd(char *buf, size_t size);
    int linux_sys_utime(const char *path, uint64_t mtime);
    int linux_sys_exists(const char *path, uint8_t *type);
    pid_t linux_sys_spawn(const char *path, char *const argv[], char *const envp[]);
    void linux_sys_exit(int status) __attribute__((noreturn));
    pid_t linux_sys_wait(pid_t pid, int *status, int options);
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
    void *macos_sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
    int macos_sys_munmap(void *addr, size_t len);
    int macos_sys_mprotect(void *addr, size_t len, int prot);
    int macos_sys_yield(void);
    pid_t macos_sys_getpid(void);
    pid_t macos_sys_getppid(void);
    int macos_sys_setpriority(pid_t pid, int prio);
    int macos_sys_pipe(int fds[2]);
    int macos_sys_dup(int oldfd);
    int macos_sys_dup2(int oldfd, int newfd);
    int macos_sys_select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tmo);
    int macos_sys_sem_wait(sem_t *sem);
    int macos_sys_sem_post(sem_t *sem);
    int macos_sys_clock_gettime(clockid_t id, struct timespec *tp);
    int macos_sys_gettimeofday(struct timeval *tv, void *tz);
    int macos_sys_nanosleep(const struct timespec *req, struct timespec *rem);
    unsigned macos_sys_sleep(unsigned seconds);
    int macos_sys_setitimer(int which, const struct itimerval *new, struct itimerval *old);
    int macos_sys_get_file_info(const char *path, kora_file_info_t *info);
    int macos_sys_get_fd_info(int fd, kora_file_info_t *info);
    int macos_sys_stat(const char *path, kora_stat_t *st);
    int macos_sys_fstat(int fd, kora_stat_t *st);
    int macos_sys_lstat(const char *path, kora_stat_t *st);
    int macos_sys_link(const char *existing, const char *newpath);
    int macos_sys_chdir(const char *path);
    int macos_sys_getcwd(char *buf, size_t size);
    int macos_sys_utime(const char *path, uint64_t mtime);
    int macos_sys_exists(const char *path, uint8_t *type);
    pid_t macos_sys_spawn(const char *path, char *const argv[], char *const envp[]);
    void macos_sys_exit(int status) __attribute__((noreturn));
    pid_t macos_sys_wait(pid_t pid, int *status, int options);
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
    void *windows_sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
    int windows_sys_munmap(void *addr, size_t len);
    int windows_sys_mprotect(void *addr, size_t len, int prot);
    int windows_sys_yield(void);
    pid_t windows_sys_getpid(void);
    pid_t windows_sys_getppid(void);
    int windows_sys_setpriority(pid_t pid, int prio);
    int windows_sys_pipe(int fds[2]);
    int windows_sys_dup(int oldfd);
    int windows_sys_dup2(int oldfd, int newfd);
    int windows_sys_select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tmo);
    int windows_sys_sem_wait(sem_t *sem);
    int windows_sys_sem_post(sem_t *sem);
    int windows_sys_clock_gettime(clockid_t id, struct timespec *tp);
    int windows_sys_gettimeofday(struct timeval *tv, void *tz);
    int windows_sys_nanosleep(const struct timespec *req, struct timespec *rem);
    unsigned windows_sys_sleep(unsigned seconds);
    int windows_sys_setitimer(int which, const struct itimerval *new, struct itimerval *old);
    int windows_sys_get_file_info(const char *path, kora_file_info_t *info);
    int windows_sys_get_fd_info(int fd, kora_file_info_t *info);
    int windows_sys_stat(const char *path, kora_stat_t *st);
    int windows_sys_fstat(int fd, kora_stat_t *st);
    int windows_sys_lstat(const char *path, kora_stat_t *st);
    int windows_sys_link(const char *existing, const char *newpath);
    int windows_sys_chdir(const char *path);
    int windows_sys_getcwd(char *buf, size_t size);
    int windows_sys_utime(const char *path, uint64_t mtime);
    int windows_sys_exists(const char *path, uint8_t *type);
    pid_t windows_sys_spawn(const char *path, char *const argv[], char *const envp[]);
    void windows_sys_exit(int status);
    pid_t windows_sys_wait(pid_t pid, int *status, int options);
#endif
