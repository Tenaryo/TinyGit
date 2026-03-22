#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace git {

class Blob {
  public:
    static std::expected<std::string, std::string> extract_content(std::string_view raw_data);
    static std::expected<std::string, std::string> create_blob_data(std::string_view content);
};

} // namespace git
