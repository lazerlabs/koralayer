# KoraLayer

KoraLayer is a compatibility layer for simulating KoraOS system calls on host operating systems. This allows developing applications for KoraOS without waiting for the full kernel to be ready.

## Features

- Cross-platform compatibility layer for KoraOS syscalls
- Simple, clean API matching future KoraOS design
- Implements basic file I/O syscalls (open, close, read, write, seek, ioctl)
- Supports directory operations (mkdir, rmdir, opendir, readdir, closedir)
- Provides symbolic link functionality (symlink, readlink)
- Currently supports Linux with placeholders for macOS and Windows

## Project Structure
```
koralayer/
├── include/ # Public headers
├── src/ # Source code
├── tests/ # Test programs
├── docs/ # Documentation
├── lib/ # Built libraries (generated)
└── build/ # Build artifacts (generated)
```
## System Call Reference

See [syscalls.md](syscalls.md) for a complete reference of all implemented system calls.

## Documentation

See the [docs](docs/) directory for detailed documentation.

## Building

KoraLayer uses CMake as its build system. You can build the project using the provided build script or manually with CMake.

### Prerequisites

- CMake (version 3.10 or later)
- C compiler (GCC, Clang, or MSVC)
- [CMocka](https://cmocka.org/) (for running tests)

### Building with the Build Script

The `build.sh` script wraps all common build and test operations:

```bash
# Basic debug build
./build.sh

# Clean build
./build.sh --clean

# Build in release mode
./build.sh --release

# Run tests after building
./build.sh --test

# Enable verbose output
./build.sh --verbose

# Specify a custom build directory
./build.sh --build-dir mybuild
```

You can combine options, e.g.:
```bash
./build.sh --clean --release --test
```

### Manual CMake Build

If you prefer to use CMake directly:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running Tests

To run the tests after building:

```bash
# Using the build script
./build.sh --test

# Or manually, from the build directory
cd build
ctest

# For verbose test output
CMOCKA_MESSAGE_OUTPUT=stdout ctest -V
```

## Cross-Platform Support

KoraLayer supports Linux, macOS, and Windows (via MinGW or MSVC). The Linux implementation is complete; macOS and Windows support are in progress.

## Current Status

- Basic infrastructure is in place
- File I/O and directory operations implemented for Linux
- Symbolic link operations supported
- Only Linux implementation is complete; other platforms are under development 