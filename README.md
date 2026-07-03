# Competitive Repository

This directory contains the complete C++23 code that supports the Quarto book.

- Complete C++23 programs: 380
- Files with `main`: 380
- Canonical layout: `cap01/` through `cap16/`, plus `cap18/` for the training program

The book keeps only the smallest useful excerpt inline. Longer implementations, benchmarks, brute-force validators, and complete programs live here.

## Compile

Most complete examples can still be compiled individually with a C++23 compiler:

```bash
clang++-18 -std=c++23 -stdlib=libc++ -O2 path/to/example.cpp -o example
```

On WSL/Linux, use the repository helper to compile the whole set:

```bash
bash compiling.sh
```

The script probes the available C++23 toolchains once, then chooses the first
compiler/standard-library pair that supports the features used by each file. In
practice, on a Windows 11 + WSL + LLVM setup, the default preference is:

1. `clang++` with `libc++`
2. `clang++` with `libc++` and `-fexperimental-library`
3. `clang++` with `libstdc++`
4. `g++` with `libstdc++`

It writes `compilation-report.txt` and places generated binaries under
`.compile-check/`. Useful variants:

```bash
bash compiling.sh --clean
bash compiling.sh --preflight
LLVM_VERSION=23 bash compiling.sh
TOOLCHAIN_ORDER="clang-libc++ gcc" bash compiling.sh
```

The script compiles every `.cpp` file it finds under this directory. See
`manifest.csv` for the mechanical index.
