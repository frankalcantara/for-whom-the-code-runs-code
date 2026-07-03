// Exercise 3.6 - The Lantern Keeper's Configuration
// Read one brightness value and preserve the reason for failure.
// Time: O(file size). Space: O(1), excluding stream buffers.

#include <expected>
#include <fstream>
#include <print>
#include <string>

std::expected<int, std::string> read_brightness(const std::string& path) {
    std::ifstream input(path);
    if (!input) return std::unexpected("configuration file is missing");

    int value = 0;
    if (!(input >> value)) return std::unexpected("brightness is not an integer");
    if (value < 0 || value > 100)
        return std::unexpected("brightness must be between 0 and 100");

    std::string extra;
    if (input >> extra)
        return std::unexpected("unexpected data after brightness");
    return value;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::print("usage: {} <configuration-file>\n", argv[0]);
        return 1;
    }

    const auto brightness = read_brightness(argv[1]);
    if (!brightness) {
        std::print("ERROR: {}\n", brightness.error());
        return 1;
    }

    std::print("brightness={}\n", *brightness);
}
