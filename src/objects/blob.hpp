#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>

namespace objects {

class Blob {
  public:
    static auto
    extract_content(std::string_view raw_data) -> std::expected<std::string, std::string>;
    static auto
    create_blob_data(std::string_view content) -> std::expected<std::string, std::string>;

    static auto write_from_file(const std::filesystem::path& file_path, bool write_to_store = true)
        -> std::expected<std::string, std::string>;
};

} // namespace objects
