# For Whom the Code Runs: Code

Companion code repository for *For Whom the Code Runs*, a book about modern, high-performance C++23 taught through competitive programming.

The repository contains complete programs, benchmarks, validators, and training solutions used by the book. The text keeps only the smallest useful excerpts; the full working code lives here.

## Status

Current repository size:

- 380 complete C++23 programs
- one `main` per program
- chapters `cap01/` through `cap16/`
- training solutions in `cap18/`

The main validation target is LLVM on Ubuntu or WSL.

Current validated LLVM toolchain:

```text
clang++-23
Ubuntu clang version 23.0.0
libc++ from /usr/lib/llvm-23
-std=c++23 -O2 -Wall -Wextra -pthread
```

Current MSVC validation path:

```text
Visual Studio 18 Community
MSVC toolset 14.51.36231
/std:c++latest
/O2
```

## Build

On Ubuntu or WSL:

```bash
bash compiling.sh --preflight
bash compiling.sh
```

To force the LLVM version used by the current validation:

```bash
LLVM_VERSION=23 bash compiling.sh
```

The script probes available C++23 toolchains, checks library feature support, then compiles each source file with the first suitable compiler and standard library pair. It writes:

```text
compilation-report.txt
.compile-check/
```

## Toolchain Notes

Most programs compile with LLVM 23 and libc++.
Two programs require C++23 <generator>, which is not covered by the current LLVM/libc++ 23 path:

```text
cap07/13-generator-combinations.cpp
cap07/14-generator-benchmark.cpp
```

These are validated through the MSVC exception path, where std::generator is available.
Some examples use standard-library execution policies:

```text
cap02/05-loops.cpp
cap16/04-parallel-algorithms.cpp
cap16/11-stained-glass.cpp
```

Depending on the local standard library, these may require either LLVM/libc++ with experimental library support or the MSVC validation path.

**Platform-Specific Code**

The POSIX mmap example is Linux/WSL code:

```text
cap03/04-mmap.cpp
```

The related cap03/14-map-room.cpp compiles portably, but its mmap mode is available only on Unix-like systems.
Several files use __int128 or __uint128_t. Treat them as LLVM/GCC-family examples until a standard-compatible MSVC alternative is reviewed:

```text
cap05/06-hashing.cpp
cap05/22-plagiarist-manuscript.cpp
cap13/06-border-patrol.cpp
cap13/08-nearest-lanterns.cpp
cap13/09-surveyors-parchment.cpp
cap14/07-expedition-crates.cpp
cap18/t05-s4-2-twin-inscriptions.cpp
```

GCC may compile many of these programs, but GCC is not the current validation target for this repository.
License
The source code is released under the MIT License. See LICENSE for details.
