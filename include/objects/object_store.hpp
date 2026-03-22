#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace git {

class ObjectStore {
  public:
    static std::expected<std::string, std::string> read_object(std::string_view sha);
    static std::string object_path(std::string_view sha);

    static std::expected<std::string, std::string> decompress(const std::string& compressed);
};

} // namespace git
