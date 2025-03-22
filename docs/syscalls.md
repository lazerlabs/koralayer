# KoraOS System Call Reference

This document describes all system calls implemented in the KoraLayer compatibility layer.

## System Call Index

| Name | Number | Description |
|------|--------|-------------|
| [`sys_putc`](#sys_putc) | 1 | Write a character to standard output |
| [`sys_getc`](#sys_getc) | 2 | Read a character from standard input |
| [`sys_open`](#sys_open) | 3 | Open a file and return a file descriptor |
| [`sys_close`](#sys_close) | 4 | Close a file descriptor |
| [`sys_read`](#sys_read) | 5 | Read from a file descriptor into a buffer |
| [`sys_write`](#sys_write) | 6 | Write a buffer to a file descriptor |
| [`sys_seek`](#sys_seek) | 7 | Reposition read/write file offset |
| [`sys_ioctl`](#sys_ioctl) | 8 | Device-specific control operations |

## Common Constants

### File Open Flags
| Flag | Value | Description |
|------|-------|-------------|
| `KORA_O_RDONLY` | 0x0001 | Open for reading only |
| `KORA_O_WRONLY` | 0x0002 | Open for writing only |
| `KORA_O_RDWR` | 0x0003 | Open for reading and writing |
| `KORA_O_CREAT` | 0x0100 | Create file if it does not exist |
| `KORA_O_TRUNC` | 0x0200 | Truncate file to zero length |
| `KORA_O_APPEND` | 0x0400 | Append to the file |

### Seek Modes
| Mode | Value | Description |
|------|-------|-------------|
| `KORA_SEEK_SET` | 0 | Set position to offset |
| `KORA_SEEK_CUR` | 1 | Set position to current location plus offset |
| `KORA_SEEK_END` | 2 | Set position to EOF plus offset |

### Status/Error Codes
| Code | Value | Description |
|------|-------|-------------|
| `KORA_SUCCESS` | 0 | Operation succeeded |
| `KORA_ERROR` | -1 | Operation failed |
| `KORA_EOF` | -2 | End of file reached |

## System Call Details

### sys_putc

**System Call Number:** 1

**Prototype:**
```c
int sys_putc(char c);
```

**Description:**
Writes a single character to the standard output.

**Parameters:**
- `c`: The character to write

**Return Value:**
- `KORA_SUCCESS` (0) on success
- `KORA_ERROR` (-1) on failure

**Example:**
```c
#include <kora/syscalls.h>

int main() {
    sys_putc('H');
    sys_putc('i');
    sys_putc('\n');
    return 0;
}
```

**Notes:**
- This is a simple syscall used for testing the KoraLayer infrastructure.
- In the real KoraOS, this would interface with the console driver.

**Implementation Notes:**
- Linux: Uses glibc `putchar()` function
- macOS: Not yet implemented
- Windows: Not yet implemented

### sys_getc

**System Call Number:** 2

**Prototype:**
```c
int sys_getc(void);
```

**Description:**
Reads a single character from standard input.

**Parameters:**
- None

**Return Value:**
- The character read (0-255)
- `KORA_EOF` (-2) on end of file
- `KORA_ERROR` (-1) on error

**Example:**
```c
#include <kora/syscalls.h>

int main() {
    int c = sys_getc();
    if (c >= 0) {
        sys_putc((char)c);  // Echo the character
    }
    return 0;
}
```

**Implementation Notes:**
- Linux: Uses glibc `getchar()` function
- macOS: Not yet implemented
- Windows: Not yet implemented

### sys_open

**System Call Number:** 3

**Prototype:**
```c
int sys_open(const char *path, int flags);
```

**Description:**
Opens a file and returns a file descriptor.

**Parameters:**
- `path`: Path to the file
- `flags`: Flags controlling how the file is opened (see File Open Flags)

**Return Value:**
- File descriptor (non-negative) on success
- `KORA_ERROR` (-1) on failure

**Example:**
```c
#include <kora/syscalls.h>

int main() {
    int fd = sys_open("example.txt", KORA_O_RDWR | KORA_O_CREAT);
    if (fd >= 0) {
        // Use the file descriptor
        sys_close(fd);
    }
    return 0;
}
```

**Implementation Notes:**
- Linux: Uses glibc `open()` function
- macOS: Not yet implemented
- Windows: Not yet implemented

### sys_close

**System Call Number:** 4

**Prototype:**
```c
int sys_close(int fd);
```

**Description:**
Closes a file descriptor.

**Parameters:**
- `fd`: File descriptor to close

**Return Value:**
- `KORA_SUCCESS` (0) on success
- `KORA_ERROR` (-1) on failure

**Example:**
```c
#include <kora/syscalls.h>

int main() {
    int fd = sys_open("example.txt", KORA_O_RDONLY);
    if (fd >= 0) {
        // Use the file descriptor
        sys_close(fd);
    }
    return 0;
}
```

**Implementation Notes:**
- Linux: Uses glibc `close()` function
- macOS: Not yet implemented
- Windows: Not yet implemented

### sys_read

**System Call Number:** 5

**Prototype:**
```c
int sys_read(int fd, void *buf, size_t count);
```

**Description:**
Reads data from a file descriptor into a buffer.

**Parameters:**
- `fd`: File descriptor to read from
- `buf`: Buffer to read into
- `count`: Number of bytes to read

**Return Value:**
- Number of bytes read (non-negative) on success
- `KORA_EOF` (-2) on end of file
- `KORA_ERROR` (-1) on error

**Example:**
```c
#include <kora/syscalls.h>

int main() {
    int fd = sys_open("example.txt", KORA_O_RDONLY);
    if (fd >= 0) {
        char buffer[100];
        int bytes_read = sys_read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read >= 0) {
            buffer[bytes_read] = '\0';  // Null terminate
        }
        sys_close(fd);
    }
    return 0;
}
```

**Implementation Notes:**
- Linux: Uses glibc `read()` function
- macOS: Not yet implemented
- Windows: Not yet implemented

### sys_write

**System Call Number:** 6

**Prototype:**
```c
int sys_write(int fd, const void *buf, size_t count);
```

**Description:**
Writes data from a buffer to a file descriptor.

**Parameters:**
- `fd`: File descriptor to write to
- `buf`: Buffer to write from
- `count`: Number of bytes to write

**Return Value:**
- Number of bytes written (non-negative) on success
- `KORA_ERROR` (-1) on error

**Example:**
```c
#include <kora/syscalls.h>
#include <string.h>

int main() {
    int fd = sys_open("example.txt", KORA_O_WRONLY | KORA_O_CREAT | KORA_O_TRUNC);
    if (fd >= 0) {
        const char *message = "Hello, KoraOS!";
        sys_write(fd, message, strlen(message));
        sys_close(fd);
    }
    return 0;
}
```

**Implementation Notes:**
- Linux: Uses glibc `write()` function
- macOS: Not yet implemented
- Windows: Not yet implemented

### sys_seek

**System Call Number:** 7

**Prototype:**
```c
long sys_seek(int fd, long offset, int whence);
```

**Description:**
Repositions the read/write file offset.

**Parameters:**
- `fd`: File descriptor
- `offset`: Offset from origin
- `whence`: Position origin (KORA_SEEK_SET, KORA_SEEK_CUR, KORA_SEEK_END)

**Return Value:**
- New offset from beginning of file (non-negative) on success
- `KORA_ERROR` (-1) on error

**Example:**
```c
#include <kora/syscalls.h>

int main() {
    int fd = sys_open("example.txt", KORA_O_RDWR);
    if (fd >= 0) {
        // Seek to 10 bytes from the beginning
        sys_seek(fd, 10, KORA_SEEK_SET);
        
        // Go back 5 bytes from current position
        sys_seek(fd, -5, KORA_SEEK_CUR);
        
        // Go to end of file
        sys_seek(fd, 0, KORA_SEEK_END);
        
        sys_close(fd);
    }
    return 0;
}
```

**Implementation Notes:**
- Linux: Uses glibc `lseek()` function
- macOS: Not yet implemented
- Windows: Not yet implemented

### sys_ioctl

**System Call Number:** 8

**Prototype:**
```c
int sys_ioctl(int fd, unsigned long request, void *arg);
```

**Description:**
Performs device-specific control operations.

**Parameters:**
- `fd`: File descriptor for device
- `request`: Device-specific request code
- `arg`: Optional argument depending on request

**Return Value:**
- `KORA_SUCCESS` (0) on success
- `KORA_ERROR` (-1) on error

**Example:**
```c
#include <kora/syscalls.h>

// Example: Get terminal window size
struct winsize {
    unsigned short ws_row;
    unsigned short ws_col;
    unsigned short ws_xpixel;
    unsigned short ws_ypixel;
};

int main() {
    struct winsize ws;
    if (sys_ioctl(0, 0x5413 /* TIOCGWINSZ */, &ws) == KORA_SUCCESS) {
        // Use terminal size
    }
    return 0;
}
```

**Implementation Notes:**
- Linux: Uses glibc `ioctl()` function
- macOS: Not yet implemented
- Windows: Not yet implemented 