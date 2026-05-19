#pragma once

#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <string_view>

namespace util {

[[nodiscard]] auto hex_to_bytes(std::string_view hex) -> std::expected<std::string, std::string>;
[[nodiscard]] auto bytes_to_hex(std::span<const std::uint8_t> bytes) -> std::string;

} // namespace util
