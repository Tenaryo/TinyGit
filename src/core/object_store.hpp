#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace core {

auto store_object(const std::string& raw_data) -> std::expected<std::string, std::string>;
auto read_object(std::string_view sha) -> std::expected<std::string, std::string>;
auto compute_sha1(const std::string& data) -> std::string;
auto compress(const std::string& data) -> std::expected<std::string, std::string>;
auto decompress(const std::string& compressed) -> std::expected<std::string, std::string>;
auto object_path(std::string_view sha) -> std::string;

} // namespace core
