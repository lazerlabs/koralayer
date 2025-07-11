#include <internal/syscall_impl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/select.h>
#include <time.h>
#include <sys/time.h>

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

void *windows_sys_brk(void *new_end) {
    (void)new_end;
    /* TODO: Implement Windows version */
    return (void *)-1;
}

void *windows_sys_sbrk(ptrdiff_t delta) {
    (void)delta;
    /* TODO: Implement Windows version */
    return (void *)-1;
}

void *windows_sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off) {
    (void)addr; (void)len; (void)prot; (void)flags; (void)fd; (void)off;
    /* TODO: Implement Windows version */
    return (void *)-1;
}

int windows_sys_munmap(void *addr, size_t len) {
    (void)addr; (void)len;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_mprotect(void *addr, size_t len, int prot) {
    (void)addr; (void)len; (void)prot;
    /* TODO: Implement Windows version */
    return -1;
}

pid_t windows_sys_spawn(const char *path, char *const argv[], char *const envp[]) {
    (void)path; (void)argv; (void)envp;
    /* TODO: Implement Windows version */
    return -1;
}

void windows_sys_exit(int status) {
    (void)status;
    /* TODO: Implement Windows version */
}

pid_t windows_sys_wait(pid_t pid, int *status, int options) {
    (void)pid; (void)status; (void)options;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_yield(void) {
    /* TODO: Implement Windows version */
    return -1;
}

pid_t windows_sys_getpid(void) {
    /* TODO: Implement Windows version */
    return -1;
}

pid_t windows_sys_getppid(void) {
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_setpriority(pid_t pid, int prio) {
    (void)pid; (void)prio;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_pipe(int fds[2]) {
    (void)fds;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_dup(int oldfd) {
    (void)oldfd;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_dup2(int oldfd, int newfd) {
    (void)oldfd; (void)newfd;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_stat(const char *path, kora_stat_t *st) {
    (void)path; (void)st;
    return -1;
}

int windows_sys_fstat(int fd, kora_stat_t *st) {
    (void)fd; (void)st;
    return -1;
}

int windows_sys_lstat(const char *path, kora_stat_t *st) {
    (void)path; (void)st;
    return -1;
}

int windows_sys_link(const char *existing, const char *newpath) {
    (void)existing; (void)newpath;
    return -1;
}

int windows_sys_unlink(const char *path) {
    (void)path;
    return -1;
}

int windows_sys_rename(const char *oldpath, const char *newpath) {
    (void)oldpath; (void)newpath;
    return -1;
}

int windows_sys_chdir(const char *path) {
    (void)path;
    return -1;
}

int windows_sys_getcwd(char *buf, size_t size) {
    (void)buf; (void)size;
    return -1;
}

int windows_sys_utime(const char *path, uint64_t mtime) {
    (void)path; (void)mtime;
    return -1;
}

int windows_sys_select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tmo) {
    (void)nfds; (void)r; (void)w; (void)e; (void)tmo;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_sem_wait(sem_t *sem) {
    (void)sem;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_sem_post(sem_t *sem) {
    (void)sem;
    /* TODO: Implement Windows version */
    return -1;
}

int windows_sys_clock_gettime(clockid_t id, struct timespec *tp) {
    (void)id; (void)tp;
    return -1;
}

int windows_sys_gettimeofday(struct timeval *tv, void *tz) {
    (void)tv; (void)tz;
    return -1;
}

int windows_sys_nanosleep(const struct timespec *req, struct timespec *rem) {
    (void)req; (void)rem;
    return -1;
}

unsigned windows_sys_sleep(unsigned seconds) {
    (void)seconds;
    return seconds;
}

int windows_sys_setitimer(int which, const struct itimerval *new, struct itimerval *old) {
    (void)which; (void)new; (void)old;
    return -1;
}
#endif
