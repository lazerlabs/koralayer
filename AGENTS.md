# GPT Codex Guidelines

* **Language:** Use C11 for source code and Markdown for documentation.
* **Style:** Match the existing code style (4-space indentation, pedantic compiler flags).
* **Build & Test:** After modifying code, run `./build.sh --test` to verify the build and unit tests. Include the results in your PR description.
* **Goals:** Maintain cross-platform support (Linux, macOS and Windows shims) for the KoraOS syscall layer and koralibc. Keep documentation in `docs/` up to date with `include/kora/` headers.
