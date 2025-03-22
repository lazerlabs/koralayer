# KoraLayer

KoraLayer is a compatibility layer for simulating KoraOS system calls on host operating systems. This allows developing applications for KoraOS without waiting for the full kernel to be ready.

## Quick Start

```bash
# Clone the repository (if you haven't already)
# git clone https://github.com/your-username/kora.git
# cd kora/koralayer

# Build the library and tests
make

# Run tests
make run-tests
```

## Features

- Cross-platform compatibility layer for KoraOS syscalls
- Currently supports Linux with placeholders for macOS and Windows
- Simple, clean API matching future KoraOS design

## Documentation

See the [docs](docs/) directory for detailed documentation.

## Project Structure

```
koralayer/
├── include/           # Public headers
├── src/               # Source code
├── tests/             # Test programs
├── docs/              # Documentation
├── lib/               # Built libraries (generated)
└── build/             # Build artifacts (generated)
```

## Current Status

- Basic infrastructure is in place
- Basic console and file I/O syscalls implemented as proof of concept
- Only Linux implementation is complete 