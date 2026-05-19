#include "util/hex.hpp"

#include <array>

namespace util {

auto hex_to_bytes(std::string_view hex) -> std::expected<std::string, std::string> {
    if (hex.size() % 2 != 0) {
        return std::unexpected("Hex string has odd length");
    }

    constexpr auto nibble = [](char c) -> int {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        return -1;
    };

    std::string bytes;
    bytes.reserve(hex.size() / 2);
    for (size_t i = 0; i < hex.size(); i += 2) {
        int hi = nibble(hex[i]);
        int lo = nibble(hex[i + 1]);
        if (hi < 0 || lo < 0) [[unlikely]] {
            return std::unexpected("Invalid hex character");
        }
        bytes.push_back(static_cast<char>((hi << 4) | lo));
    }
    return bytes;
}

auto bytes_to_hex(std::span<const std::uint8_t> bytes) -> std::string {
    static constexpr std::array<char, 16> kHexChars = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string hex;
    hex.reserve(bytes.size() * 2);
    for (auto byte : bytes) {
        hex.push_back(kHexChars[byte >> 4]);
        hex.push_back(kHexChars[byte & 0x0F]);
    }
    return hex;
}

} // namespace util
