// Exercise 3.7 - The Borrowed Line
// Count non-empty lines and preserve one selected line after the buffer dies.
// Time: O(file size). Space: O(file size).

#include <expected>
#include <fstream>
#include <print>
#include <string>
#include <string_view>
#include <utility>

std::expected<std::string, std::string> read_file(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input) return std::unexpected("cannot open file");

    const auto end = input.tellg();
    if (end < 0) return std::unexpected("cannot determine file size");

    std::string content(static_cast<std::size_t>(end), '\0');
    input.seekg(0);
    input.read(content.data(), static_cast<std::streamsize>(content.size()));
    if (!input && !content.empty()) return std::unexpected("read failed");
    return content;
}

std::pair<std::size_t, std::string>
inspect_lines(const std::string& content, std::size_t selected_non_empty) {
    std::size_t count = 0;
    std::string selected;
    std::string_view remaining = content;

    while (!remaining.empty()) {
        const auto newline = remaining.find('\n');
        auto line = remaining.substr(0, newline);
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);

        if (!line.empty()) {
            ++count;
            if (count == selected_non_empty) selected.assign(line);
        }

        if (newline == std::string_view::npos) break;
        remaining.remove_prefix(newline + 1);
    }
    return {count, selected};
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::print("usage: {} <file> <non-empty-line-number>\n", argv[0]);
        return 1;
    }

    const auto content = read_file(argv[1]);
    if (!content) {
        std::print("ERROR: {}\n", content.error());
        return 1;
    }

    const auto selected_number =
        static_cast<std::size_t>(std::stoull(argv[2]));
    auto [count, selected] = inspect_lines(*content, selected_number);

    std::print("non_empty={}\n", count);
    if (selected.empty()) std::print("selected=<missing>\n");
    else                  std::print("selected={}\n", selected);
}
