#include <internal/syscall_impl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdint.h>
#include <sys/mman.h>
#include <spawn.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <semaphore.h>
#include <sched.h>
#include <sys/resource.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
extern char **environ;

/**
 * Linux implementation of KoraOS system calls
 * Uses glibc rather than direct syscalls
 */

// Global array to store DIR pointers
// This is a simplification for the compatibility layer.
// In a real implementation, we would use a more sophisticated handle table
// with proper locking for thread safety and memory management.
#define MAX_DIR_HANDLES 16
static DIR* dir_handles[MAX_DIR_HANDLES] = {NULL};

/**
 * Find a free slot in the dir_handles array
 * 
 * @return Index of a free slot or -1 if none available
 */
static int find_free_dir_handle(void) {
    for (int i = 0; i < MAX_DIR_HANDLES; i++) {
        if (dir_handles[i] == NULL) {
            return i;
        }
    }
    return -1;  // No free slots
}

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

int linux_sys_mkdir(const char *path) {
    int result = mkdir(path, 0755);  /* Default directory permission */
    
    if (result < 0) {
        /* If the directory already exists, consider it a success */
        if (errno == EEXIST) {
            struct stat st;
            if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
                return KORA_SUCCESS;
            }
        }
        
        fprintf(stderr, "linux_sys_mkdir: Failed to create directory %s: %s\n", 
                path, strerror(errno));
        return KORA_ERROR;
    }
    
    return KORA_SUCCESS;
}

int linux_sys_rmdir(const char *path) {
    int result = rmdir(path);
    
    if (result < 0) {
        return KORA_ERROR;
    }
    
    return KORA_SUCCESS;
}

int linux_sys_opendir(const char *path) {
    DIR *dir = opendir(path);
    
    if (dir == NULL) {
        fprintf(stderr, "linux_sys_opendir: Failed to open directory %s: %s\n", 
                path, strerror(errno));
        return KORA_ERROR;
    }
    
    // Find a free slot in the handle table
    int handle = find_free_dir_handle();
    if (handle < 0) {
        fprintf(stderr, "linux_sys_opendir: No free DIR handle slots\n");
        closedir(dir);
        return KORA_ERROR;
    }
    
    // Store the DIR pointer in the handle table
    dir_handles[handle] = dir;
    return handle;
}

int linux_sys_readdir(int dir, kora_dirent_t *entry) {
    if (dir < 0 || dir >= MAX_DIR_HANDLES || dir_handles[dir] == NULL || entry == NULL) {
        fprintf(stderr, "linux_sys_readdir: Invalid parameters (dir=%d, entry=%p)\n", 
                dir, (void*)entry);
        return KORA_ERROR;
    }
    
    DIR *dirp = dir_handles[dir];
    struct dirent *linux_entry;
    
    errno = 0;
    linux_entry = readdir(dirp);
    
    if (linux_entry == NULL) {
        if (errno != 0) {
            fprintf(stderr, "linux_sys_readdir: Error reading directory: %s\n", 
                    strerror(errno));
            return KORA_ERROR;
        }
        return 0;  /* End of directory */
    }
    
    /* Copy the name to the Kora dirent structure */
    strncpy(entry->name, linux_entry->d_name, sizeof(entry->name) - 1);
    entry->name[sizeof(entry->name) - 1] = '\0';  /* Ensure null-termination */
    
    /* Convert Linux file type to Kora file type */
    switch (linux_entry->d_type) {
        case DT_REG:
            entry->type = KORA_DT_REG;
            break;
        case DT_DIR:
            entry->type = KORA_DT_DIR;
            break;
        case DT_LNK:
            entry->type = KORA_DT_SYMLINK;
            break;
        default:
            entry->type = KORA_DT_UNKNOWN;
            break;
    }
    
    return 1;  /* Entry read successfully */
}

int linux_sys_closedir(int dir) {
    if (dir < 0 || dir >= MAX_DIR_HANDLES || dir_handles[dir] == NULL) {
        fprintf(stderr, "linux_sys_closedir: Invalid directory handle %d\n", dir);
        return KORA_ERROR;
    }
    
    DIR *dirp = dir_handles[dir];
    int result = closedir(dirp);
    
    if (result < 0) {
        fprintf(stderr, "linux_sys_closedir: Error closing directory: %s\n", 
                strerror(errno));
        return KORA_ERROR;
    }
    
    // Clear the handle
    dir_handles[dir] = NULL;
    
    return KORA_SUCCESS;
}

int linux_sys_symlink(const char *target, const char *linkpath) {
    int result = symlink(target, linkpath);
    
    if (result < 0) {
        return KORA_ERROR;
    }
    
    return KORA_SUCCESS;
}

int linux_sys_readlink(const char *path, char *buf, size_t size) {
    ssize_t result = readlink(path, buf, size - 1);
    
    if (result < 0) {
        return KORA_ERROR;
    }
    
    /* Ensure null-termination (readlink does not add a null byte) */
    buf[result] = '\0';
    
    return (int)result;
}

/**
 * Get file information by path
 */
int linux_sys_get_file_info(const char *path, kora_file_info_t *info)
{
    if (!path || !info) {
        return -EINVAL;
    }

    struct stat st;
    if (stat(path, &st) != 0) {
        return -errno;
    }

    // Clear the structure
    memset(info, 0, sizeof(kora_file_info_t));

    // Determine file type
    if (S_ISREG(st.st_mode)) {
        info->type = KORA_FILE_TYPE_REGULAR;
    } else if (S_ISDIR(st.st_mode)) {
        info->type = KORA_FILE_TYPE_DIRECTORY;
    } else if (S_ISLNK(st.st_mode)) {
        info->type = KORA_FILE_TYPE_SYMLINK;
    } else {
        info->type = KORA_FILE_TYPE_OTHER;
    }

    // Set size and timestamps
    info->size = st.st_size;
    info->creation_time = st.st_ctime;
    info->modified_time = st.st_mtime;
    info->access_time = st.st_atime;

    // Set attributes (read-only, hidden, etc.)
    // Linux doesn't have direct equivalents, but we can approximate
    info->attributes = 0;
    if (!(st.st_mode & S_IWUSR)) {
        info->attributes |= KORA_FILE_ATTR_READONLY;
    }

    // For Linux, we'll use inode as the starting cluster just as a placeholder
    // In a real KoraOS implementation, this would be the actual starting cluster
    info->starting_cluster = st.st_ino;

    return 0;
}

/**
 * Get file information by file descriptor
 */
int linux_sys_get_fd_info(int fd, kora_file_info_t *info)
{
    if (fd < 0 || !info) {
        return -EINVAL;
    }

    struct stat st;
    if (fstat(fd, &st) != 0) {
        return -errno;
    }

    // Clear the structure
    memset(info, 0, sizeof(kora_file_info_t));

    // Determine file type
    if (S_ISREG(st.st_mode)) {
        info->type = KORA_FILE_TYPE_REGULAR;
    } else if (S_ISDIR(st.st_mode)) {
        info->type = KORA_FILE_TYPE_DIRECTORY;
    } else if (S_ISLNK(st.st_mode)) {
        info->type = KORA_FILE_TYPE_SYMLINK;
    } else {
        info->type = KORA_FILE_TYPE_OTHER;
    }

    // Set size and timestamps
    info->size = st.st_size;
    info->creation_time = st.st_ctime;
    info->modified_time = st.st_mtime;
    info->access_time = st.st_atime;

    // Set attributes (read-only, hidden, etc.)
    info->attributes = 0;
    if (!(st.st_mode & S_IWUSR)) {
        info->attributes |= KORA_FILE_ATTR_READONLY;
    }

    // For Linux, we'll use inode as the starting cluster just as a placeholder
    info->starting_cluster = st.st_ino;

    return 0;
}

int linux_sys_stat(const char *path, kora_stat_t *st)
{
    if (!path || !st) {
        return -EINVAL;
    }

    struct stat host;
    if (stat(path, &host) != 0) {
        return -errno;
    }

    st->mode = host.st_mode;
    st->size = host.st_size;
    st->mtime = host.st_mtime;
    st->uid = 0;
    st->gid = 0;

    return 0;
}

int linux_sys_fstat(int fd, kora_stat_t *st)
{
    if (fd < 0 || !st) {
        return -EINVAL;
    }

    struct stat host;
    if (fstat(fd, &host) != 0) {
        return -errno;
    }

    st->mode = host.st_mode;
    st->size = host.st_size;
    st->mtime = host.st_mtime;
    st->uid = 0;
    st->gid = 0;

    return 0;
}

int linux_sys_lstat(const char *path, kora_stat_t *st)
{
    if (!path || !st) {
        return -EINVAL;
    }

    struct stat host;
    if (lstat(path, &host) != 0) {
        return -errno;
    }

    st->mode = host.st_mode;
    st->size = host.st_size;
    st->mtime = host.st_mtime;
    st->uid = 0;
    st->gid = 0;

    return 0;
}

int linux_sys_link(const char *existing, const char *newpath)
{
    if (!existing || !newpath) {
        return -EINVAL;
    }
    if (link(existing, newpath) != 0) {
        return -errno;
    }
    return 0;
}

int linux_sys_chdir(const char *path)
{
    if (!path) {
        return -EINVAL;
    }
    if (chdir(path) != 0) {
        return -errno;
    }
    return 0;
}

int linux_sys_getcwd(char *buf, size_t size)
{
    if (!buf || size == 0) {
        return -EINVAL;
    }
    if (getcwd(buf, size) == NULL) {
        return -errno;
    }
    return 0;
}

int linux_sys_utime(const char *path, uint64_t mtime)
{
    if (!path) {
        return -EINVAL;
    }
    struct timespec times[2];
    times[0].tv_sec = (time_t)mtime;
    times[0].tv_nsec = 0;
    times[1] = times[0];
    if (utimensat(AT_FDCWD, path, times, 0) != 0) {
        return -errno;
    }
    return 0;
}

/**
 * Check if a path exists and determine its type
 */
int linux_sys_exists(const char *path, uint8_t *type)
{
    if (!path) {
        return -EINVAL;
    }

    struct stat st;
    
    // Use lstat to not follow symlinks
    if (lstat(path, &st) != 0) {
        if (errno == ENOENT) {
            // Path does not exist
            return 0;
        }
        // Error during stat
        return -errno;
    }

    // Path exists
    if (type) {
        if (S_ISREG(st.st_mode)) {
            *type = KORA_FILE_TYPE_REGULAR;
        } else if (S_ISDIR(st.st_mode)) {
            *type = KORA_FILE_TYPE_DIRECTORY;
        } else if (S_ISLNK(st.st_mode)) {
            *type = KORA_FILE_TYPE_SYMLINK;
        } else {
            *type = KORA_FILE_TYPE_OTHER;
        }
    }

    return 1; // Exists
}

/**
 * Remove a file from the filesystem
 */
int linux_sys_unlink(const char *path)
{
    if (!path) {
        return -EINVAL;
    }
    
    if (unlink(path) != 0) {
        return -errno;
    }
    
    return 0;
}

/**
 * Rename a file or directory
 */
int linux_sys_rename(const char *oldpath, const char *newpath)
{
    if (!oldpath || !newpath) {
        return -EINVAL;
    }

    if (rename(oldpath, newpath) != 0) {
        return -errno;
    }

    return 0;
}

void *linux_sys_brk(void *new_end)
{
    if (brk(new_end) != 0) {
        return (void *)-1;
    }
    return sbrk(0);
}

void *linux_sys_sbrk(ptrdiff_t delta)
{
    return sbrk(delta);
}

void *linux_sys_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
    void *res = mmap(addr, len, prot, flags, fd, off);
    if (res == MAP_FAILED) {
        return (void *)-1;
    }
    return res;
}

int linux_sys_munmap(void *addr, size_t len)
{
    return munmap(addr, len);
}

int linux_sys_mprotect(void *addr, size_t len, int prot)
{
    return mprotect(addr, len, prot);
}

pid_t linux_sys_spawn(const char *path, char *const argv[], char *const envp[])
{
    pid_t pid;
    int ret = posix_spawn(&pid, path, NULL, NULL, argv,
                          envp ? envp : environ);
    if (ret != 0) {
        errno = ret;
        return -1;
    }
    return pid;
}

void linux_sys_exit(int status)
{
    _exit(status);
}

pid_t linux_sys_wait(pid_t pid, int *status, int options)
{
    pid_t res = waitpid(pid, status, options);
    if (res < 0) {
        return -1;
    }
    return res;
}

int linux_sys_yield(void)
{
    return sched_yield();
}

pid_t linux_sys_getpid(void)
{
    return getpid();
}

pid_t linux_sys_getppid(void)
{
    return getppid();
}

int linux_sys_setpriority(pid_t pid, int prio)
{
    return setpriority(PRIO_PROCESS, pid, prio);
}

int linux_sys_pipe(int fds[2])
{
#ifdef __linux__
#ifdef O_NONBLOCK
    if (pipe2(fds, O_NONBLOCK) == 0) {
        return 0;
    }
#endif
#endif
    if (pipe(fds) != 0) {
        return -1;
    }
    fcntl(fds[0], F_SETFL, fcntl(fds[0], F_GETFL) | O_NONBLOCK);
    fcntl(fds[1], F_SETFL, fcntl(fds[1], F_GETFL) | O_NONBLOCK);
    return 0;
}

int linux_sys_dup(int oldfd)
{
    return dup(oldfd);
}

int linux_sys_dup2(int oldfd, int newfd)
{
    return dup2(oldfd, newfd);
}

int linux_sys_select(int nfds, fd_set *r, fd_set *w, fd_set *e, struct timeval *tmo)
{
    return select(nfds, r, w, e, tmo);
}

int linux_sys_sem_wait(sem_t *sem)
{
    return sem_wait(sem);
}

int linux_sys_sem_post(sem_t *sem)
{
    return sem_post(sem);
}

int linux_sys_clock_gettime(clockid_t id, struct timespec *tp)
{
    return clock_gettime(id, tp);
}

int linux_sys_gettimeofday(struct timeval *tv, void *tz)
{
    (void)tz;
    return gettimeofday(tv, NULL);
}

int linux_sys_nanosleep(const struct timespec *req, struct timespec *rem)
{
    return nanosleep(req, rem);
}

unsigned linux_sys_sleep(unsigned seconds)
{
    struct timespec req = { (time_t)seconds, 0 };
    return nanosleep(&req, NULL);
}

int linux_sys_setitimer(int which, const struct itimerval *new, struct itimerval *old)
{
    return setitimer(which, new, old);
}

sighandler_t linux_sys_signal(int signum, sighandler_t handler)
{
    return signal(signum, handler);
}

int linux_sys_kill(pid_t pid, int signum)
{
    return kill(pid, signum);
}

int linux_sys_sigreturn(void)
{
    return 0;
}

int linux_sys_sync(void)
{
    sync();
    return 0;
}

int linux_sys_reboot(int cmd)
{
    (void)cmd;
    errno = ENOSYS;
    return -1;
}

int linux_sys_mount(const char *src, const char *tgt, const char *type,
                    unsigned flags, const void *data)
{
    (void)src;
    (void)tgt;
    (void)type;
    (void)flags;
    (void)data;
    errno = ENOSYS;
    return -1;
}

