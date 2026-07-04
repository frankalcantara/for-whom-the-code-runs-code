# For Whom the Code Runs: Code

Companion C++23 code repository for *For Whom the Code Runs*, a book about modern, high-performance C++23 taught through competitive programming.

The book keeps only the smallest useful excerpts inline. The full implementations, benchmarks, brute-force validators, and training solutions live here.

## Contents

- Complete C++23 programs: 380
- Files with `main`: 380
- Canonical layout: `cap01/` through `cap16`
- Training program solutions: `cap18/`
- Mechanical index: `manifest.csv`

## Build

On Ubuntu, WSL, or Git Bash for Windows, use the repository helper:

```bash
bash compiling.sh --preflight
bash compiling.sh
```

The script probes available C++23 toolchains, checks library feature support, then compiles each source file with the first suitable compiler and standard-library pair. When Visual Studio is available on Windows, it also discovers `cl.exe` through `VsDevCmd.bat` and uses MSVC for the C++23 library cases that need it. It writes `compilation-report.txt` and places generated binaries under `.compile-check/`.

Command-line options:

```text
--preflight  Probe the available toolchains and stop before compiling sources.
--clean      Remove the build directory before compiling.
--help       Show the script help.
```

Useful environment overrides:

```text
LLVM_VERSION     Prefer clang++-$LLVM_VERSION when present.
GCC_VERSION      Prefer g++-$GCC_VERSION when present.
MSVC_VSDEVCMD    Path to VsDevCmd.bat when auto-discovery is not enough.
BUILD_DIR        Output directory. Default: .compile-check.
REPORT           Report file. Default: compilation-report.txt.
TOOLCHAIN_ORDER  Space-separated candidate order.
```

Examples:

```bash
LLVM_VERSION=23 bash compiling.sh
bash compiling.sh --clean
BUILD_DIR=.compile-check-msvc REPORT=msvc-report.txt TOOLCHAIN_ORDER=msvc bash compiling.sh
```

Most individual programs can also be compiled directly:

```bash
clang++-23 -std=c++23 -stdlib=libc++ -O2 path/to/example.cpp -o example
```

## Validated Toolchains

The main validation target is LLVM on Ubuntu or WSL:

```text
clang++-23
Ubuntu clang version 23.0.0
libc++ from /usr/lib/llvm-23
-std=c++23 -O2 -Wall -Wextra -pthread
```

The MSVC validation path used for C++23 library exceptions is:

```text
Visual Studio 18 Community
MSVC toolset 14.51.36231
Microsoft C/C++ compiler 19.51.36246
/std:c++latest
/O2
```

GCC may compile many programs, especially the LLVM/GCC-family examples, but GCC is not the current validation target for this repository.

## Toolchain Notes

Most programs compile with LLVM 23 and libc++.

Two programs require C++23 `<generator>`, which is not covered by the current LLVM/libc++ 23 path:

```text
cap07/13-generator-combinations.cpp
cap07/14-generator-benchmark.cpp
```

These are validated through the MSVC exception path, where `std::generator` is available.

Some examples use standard-library execution policies:

```text
cap02/05-loops.cpp
cap16/04-parallel-algorithms.cpp
cap16/11-stained-glass.cpp
```

Depending on the local standard library, these may require LLVM/libc++ with experimental library support or the MSVC validation path.

## Platform-Specific Code

The POSIX `mmap` example is Linux/WSL code:

```text
cap03/04-mmap.cpp
```

The related `cap03/14-map-room.cpp` compiles portably, but its `mmap` mode is available only on Unix-like systems.

Several files use `__int128` or `__uint128_t`. Treat them as LLVM/GCC-family examples until a standard-compatible MSVC alternative is reviewed:

```text
cap05/06-hashing.cpp
cap05/22-plagiarist-manuscript.cpp
cap13/06-border-patrol.cpp
cap13/08-nearest-lanterns.cpp
cap13/09-surveyors-parchment.cpp
cap14/07-expedition-crates.cpp
cap18/t05-s4-2-twin-inscriptions.cpp
```

## License

The source code is released under the MIT License. See `LICENSE` for details.
