// Exercise 3.5 - The Map Room
// Compute a byte checksum through fread or mmap.
// Time: O(file size). Space: O(buffer size) for fread, O(1) explicit for mmap.

#include <array>
#include <cstdio>
#include <expected>
#include <print>
#include <string>
#include <string_view>

#if defined(__unix__) || defined(__APPLE__)
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

std::expected<unsigned long long, std::string>
checksum_fread(const std::string& path) {
    std::FILE* file = std::fopen(path.c_str(), "rb");
    if (!file) return std::unexpected("cannot open file");

    std::array<unsigned char, 1U << 16> buffer{};
    unsigned long long checksum = 0;

    while (const auto count = std::fread(buffer.data(), 1, buffer.size(), file))
        for (std::size_t i = 0; i < count; ++i) checksum += buffer[i];

    if (std::ferror(file)) {
        std::fclose(file);
        return std::unexpected("read failed");
    }

    std::fclose(file);
    return checksum;
}

std::expected<unsigned long long, std::string>
checksum_mmap(const std::string& path) {
#if defined(__unix__) || defined(__APPLE__)
    const int descriptor = open(path.c_str(), O_RDONLY);
    if (descriptor < 0) return std::unexpected("cannot open file");

    struct stat info {};
    if (fstat(descriptor, &info) != 0) {
        close(descriptor);
        return std::unexpected("cannot inspect file");
    }
    if (info.st_size == 0) {
        close(descriptor);
        return 0ULL;
    }

    void* mapped = mmap(nullptr, static_cast<std::size_t>(info.st_size),
                        PROT_READ, MAP_PRIVATE, descriptor, 0);
    close(descriptor);
    if (mapped == MAP_FAILED) return std::unexpected("mapping failed");

    const auto* bytes = static_cast<const unsigned char*>(mapped);
    unsigned long long checksum = 0;
    for (off_t i = 0; i < info.st_size; ++i) checksum += bytes[i];

    munmap(mapped, static_cast<std::size_t>(info.st_size));
    return checksum;
#else
    static_cast<void>(path);
    return std::unexpected("mmap is unavailable on this platform");
#endif
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::print("usage: {} <mode-a|mode-b> <file>\n", argv[0]);
        return 1;
    }

    const std::string_view mode = argv[1];
    const auto result = mode == "mode-b"
        ? checksum_mmap(argv[2])
        : checksum_fread(argv[2]);

    if (!result) {
        std::print("ERROR: {}\n", result.error());
        return 1;
    }
    std::print("{}\n", *result);
}
