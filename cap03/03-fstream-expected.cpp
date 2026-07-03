// cap03/03-fstream-expected.cpp
// File I/O with std::ifstream and std::expected for error handling (C++23)
// Compile: g++ -O2 -std=c++23 -Wall -Wextra -o fstream_exp 03-fstream-expected.cpp

#include <expected>
#include <fstream>
#include <print>
#include <string>
#include <vector>

// ---- std::expected<T, E> (C++23) ----
// Returns either a value of type T (success) or an error of type E (failure).
// Avoids exceptions; the caller checks the result explicitly.
// .value()   — access T; throws bad_expected_access if in error state
// .error()   — access E; undefined if in value state
// .has_value() / operator bool — check which state

// Read entire file into a string. Returns std::unexpected(message) on failure.
// Time: O(file_size)
std::expected<std::string, std::string>
read_file(const std::string& path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate); // open at end
    if (!f.is_open())
        return std::unexpected("cannot open: " + path);

    auto size = f.tellg();              // file size (we opened at end)
    if (size < 0)
        return std::unexpected("cannot stat: " + path);

    std::string content(static_cast<std::size_t>(size), '\0');
    f.seekg(0);                         // rewind to beginning
    f.read(content.data(), size);       // one read call — O(size)
    if (!f)
        return std::unexpected("read failed: " + path);

    return content;                     // success
}

// Parse integers from whitespace-separated text. O(n) where n = character count.
std::vector<int> parse_ints(const std::string& text) {
    std::vector<int> result;
    std::size_t i = 0, len = text.size();
    while (i < len) {
        while (i < len && (text[i] < '0' || text[i] > '9') && text[i] != '-') ++i;
        if (i >= len) break;
        int sign = 1;
        if (text[i] == '-') { sign = -1; ++i; }
        int val = 0;
        while (i < len && text[i] >= '0' && text[i] <= '9')
            val = val * 10 + (text[i++] - '0');
        result.push_back(sign * val);
    }
    return result;
}

// Write integers to a file, one per line. Returns error string on failure.
// Time: O(n)
std::expected<void, std::string>
write_ints(const std::string& path, const std::vector<int>& data) {
    std::ofstream f(path);
    if (!f.is_open())
        return std::unexpected("cannot open for write: " + path);
    for (int x : data) f << x << '\n';
    if (!f)
        return std::unexpected("write failed: " + path);
    return {};                          // std::expected<void, E> success = {}
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::print("Usage: {} <input_file>\n", argv[0]);
        return 1;
    }

    // Read file — check result before using value
    auto content = read_file(argv[1]);
    if (!content) {
        std::print("Error: {}\n", content.error());
        return 1;
    }

    auto nums = parse_ints(*content);
    std::print("read {} integers\n", nums.size());

    long long sum = 0;
    for (int x : nums) sum += x;
    std::print("sum = {}\n", sum);

    // Write results — chained error handling with monadic std::expected
    auto result = write_ints("/tmp/output.txt", nums);
    if (!result) {
        std::print("Write error: {}\n", result.error());
        return 1;
    }
    std::print("written to /tmp/output.txt\n");
    return 0;
}
