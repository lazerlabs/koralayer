# KoraLayer Documentation

## Overview

KoraLayer is a compatibility layer that mimics Kora's syscall API using Linux/MacOS/Windows as a backend. This allows development of higher-level components without waiting for the full kernel to be ready.

## System Call Reference

See [syscalls.md](syscalls.md) for a complete reference of all implemented system calls.

## Building

```bash
# Build the library and tests
make

# Run tests
make run-tests
```

## Platform Support

KoraLayer currently supports:
- ✅ Linux (fully implemented)
- ⚠️ macOS (placeholder only)
- ⚠️ Windows (placeholder only)

## Directory Structure

- `include/` - Public and internal headers
- `src/` - Source code for the library
  - `src/linux/` - Linux-specific implementations
  - `src/macos/` - macOS-specific implementations
  - `src/windows/` - Windows-specific implementations
- `tests/` - Test programs
- `docs/` - Documentation
- `lib/` - Built libraries
- `build/` - Build artifacts 