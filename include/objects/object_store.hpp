#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace git {

class ObjectStore {
  public:
    static std::expected<std::string, std::string> read_object(std::string_view sha);
    static std::expected<void, std::string> write_object(std::string_view sha,
                                                         const std::string& compressed_data);

    static std::string object_path(std::string_view sha);

    static std::string compress(const std::string& data);
    static std::expected<std::string, std::string> decompress(const std::string& compressed);

    static std::string compute_sha1(const std::string& data);
};

} // namespace git
