# KoraLibc Implementation Plan

This document outlines the remaining work required to flesh out the KoraOS C library on top of the shim provided by KoraLayer.

## 1. Timers & Clocks
- Basic timer syscalls (`sys_clock_gettime`, `sys_gettimeofday`, `sys_nanosleep`, `sleep()`, and `sys_setitimer`) implemented.

## 2. File-System Metadata & Maintenance
- `stat` family (`sys_stat`, `sys_fstat`, `sys_lstat`).
- Link and name operations (`sys_link`, `sys_unlink`, `sys_rename`).
- Directory management (`sys_chdir`, `sys_getcwd`).
- Timestamp updates via `sys_utime`.

## 3. Signals & Exceptions
- Basic signal handling with `sys_signal`, `sys_kill`, and `sys_sigreturn`.

## 4. Device & Power Management (Stretch)
- Stubs for `sys_sync`, `sys_reboot`, and `sys_mount`.

Each subsystem should be implemented in a cross‑platform manner with a Linux reference backend.  macOS and Windows shims can be added gradually as the platform code matures.
