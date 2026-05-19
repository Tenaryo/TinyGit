#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace git {

class ObjectStore {
  public:
    static auto read_object(std::string_view sha) -> std::expected<std::string, std::string>;
    static auto write_object(std::string_view sha, const std::string& compressed_data)
        -> std::expected<void, std::string>;

    static auto object_path(std::string_view sha) -> std::string;

    static auto compress(const std::string& data) -> std::string;
    static auto
    decompress(const std::string& compressed) -> std::expected<std::string, std::string>;

    static auto compute_sha1(const std::string& data) -> std::string;
};

} // namespace git
