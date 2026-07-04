// cap03/04-mmap.cpp
// Memory-mapped file I/O — Linux/macOS only (POSIX)
// Maps the file into virtual address space: no read() calls, kernel handles paging.
// Compile (LLVM): clang++-23 -std=c++23 -stdlib=libc++ -O2 -Wall -Wextra -o mmap_io 04-mmap.cpp
// Test:    echo "5 1 2 3 4 5" > /tmp/test.txt && ./mmap_io /tmp/test.txt

#include <cstdio>
#include <cstring>
#include <print>
#include <sys/mman.h>   // mmap, munmap, MAP_*, PROT_*
#include <sys/stat.h>   // fstat, struct stat
#include <fcntl.h>      // open, O_RDONLY
#include <unistd.h>     // close

// Map entire file into memory. Returns {ptr, size}.
// On error, ptr = MAP_FAILED.
// Time: O(1) — kernel maps the file, actual reads are demand-paged.
struct MmapView {
    const char* data;
    std::size_t size;
};

MmapView mmap_open(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd < 0) return {static_cast<const char*>(MAP_FAILED), 0};

    struct stat st{};
    fstat(fd, &st);
    std::size_t sz = static_cast<std::size_t>(st.st_size);

    // MAP_PRIVATE: changes do not propagate to file.
    // PROT_READ: read-only access.
    void* ptr = mmap(nullptr, sz, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);                          // fd can be closed after mmap

    // MADV_SEQUENTIAL: hint to kernel — prefetch pages sequentially.
    madvise(ptr, sz, MADV_SEQUENTIAL);

    return {static_cast<const char*>(ptr), sz};
}

void mmap_close(MmapView v) {
    munmap(const_cast<char*>(v.data), v.size);
}

// Parse one non-negative integer starting at pos. Advances pos past the number.
// O(digits)
int parse_int(const char* buf, std::size_t& pos, std::size_t size) {
    while (pos < size && (buf[pos] < '0' || buf[pos] > '9')) ++pos;
    int val = 0;
    while (pos < size && buf[pos] >= '0' && buf[pos] <= '9')
        val = val * 10 + (buf[pos++] - '0');
    return val;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::print("Usage: {} <file>\n", argv[0]);
        return 1;
    }

    auto v = mmap_open(argv[1]);
    if (v.data == MAP_FAILED) {
        std::print("mmap failed for {}\n", argv[1]);
        return 1;
    }

    std::size_t pos = 0;
    int n = parse_int(v.data, pos, v.size);

    long long sum = 0;
    for (int i = 0; i < n; ++i)
        sum += parse_int(v.data, pos, v.size);

    mmap_close(v);
    std::print("n={} sum={}\n", n, sum);
    return 0;
}
