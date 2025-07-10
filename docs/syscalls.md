# KoraOS System Call Summary

The table below lists all system calls currently exposed by `kora/syscalls.h`. They map onto host operating system functionality through platform specific backends in `src/`.

| Number | Name | Description |
|-------:|------|-------------|
| 1 | `sys_putc` | Write a character to standard output |
| 2 | `sys_getc` | Read a character from standard input |
| 3 | `sys_open` | Open a file and return a descriptor |
| 4 | `sys_close` | Close a file descriptor |
| 5 | `sys_read` | Read from a descriptor |
| 6 | `sys_write` | Write to a descriptor |
| 7 | `sys_seek` | Reposition read/write offset |
| 8 | `sys_ioctl` | Device‑specific control operations |
| 9 | `sys_mkdir` | Create a directory |
| 10 | `sys_rmdir` | Remove a directory |
| 11 | `sys_opendir` | Open a directory for reading |
| 12 | `sys_readdir` | Read a directory entry |
| 13 | `sys_closedir` | Close a directory handle |
| 14 | `sys_symlink` | Create a symbolic link |
| 15 | `sys_readlink` | Read the target of a symbolic link |
| 16 | `sys_unlink` | Delete a filesystem entry |
| 17 | `sys_get_file_info` | Get file information by path |
| 18 | `sys_get_fd_info` | Get file information by descriptor |
| 19 | `sys_exists` | Check if a path exists |
| 20 | `sys_rename` | Rename a file or directory |
| 21 | `sys_brk` | Set program break |
| 22 | `sys_sbrk` | Adjust program break |
| 23 | `sys_mmap` | Map memory pages |
| 24 | `sys_munmap` | Unmap memory pages |
| 25 | `sys_mprotect` | Change memory protection |
| 26 | `sys_spawn` | Spawn a new process |
| 27 | `sys_exit` | Terminate the calling task |
| 28 | `sys_wait` | Wait for a child process |
| 29 | `sys_yield` | Yield the processor |
| 30 | `sys_getpid` | Get current process ID |
| 31 | `sys_getppid` | Get parent process ID |
| 32 | `sys_setpriority` | Set scheduling priority |
| 33 | `sys_pipe` | Create an anonymous pipe |
| 34 | `sys_dup` | Duplicate a file descriptor |
| 35 | `sys_dup2` | Duplicate to a specific descriptor |
| 36 | `sys_select` | Wait for descriptor readiness |
| 37 | `sys_sem_wait` | Wait on a semaphore |
| 38 | `sys_sem_post` | Post to a semaphore |

`kora/syscalls.h` also defines constants for open flags, seek modes, status codes and directory entry types.  Those are mirrored in the header and should be used when porting applications.
