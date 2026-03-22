#include "objects/blob.hpp"
#include "objects/object_store.hpp"

#include <fstream>

namespace git {

std::expected<std::string, std::string> Blob::extract_content(std::string_view raw_data) {
    size_t null_pos = raw_data.find('\0');
    if (null_pos == std::string_view::npos) {
        return std::unexpected("Invalid blob format: no null byte found");
    }

    std::string_view header = raw_data.substr(0, null_pos);
    if (!header.starts_with("blob ")) {
        return std::unexpected("Invalid blob format: expected 'blob' prefix");
    }

    return std::string(raw_data.substr(null_pos + 1));
}

std::expected<std::string, std::string> Blob::create_blob_data(std::string_view content) {
    std::string header = "blob " + std::to_string(content.size()) + '\0';
    return header + std::string(content);
}

std::expected<std::string, std::string>
Blob::write_from_file(const std::filesystem::path& file_path, bool write_to_store) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        return std::unexpected("Failed to open file: " + file_path.string());
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    auto blob_data = create_blob_data(content);
    if (!blob_data) {
        return std::unexpected(blob_data.error());
    }

    std::string sha = ObjectStore::compute_sha1(*blob_data);

    if (write_to_store) {
        std::string compressed = ObjectStore::compress(*blob_data);
        auto result = ObjectStore::write_object(sha, compressed);
        if (!result) {
            return std::unexpected(result.error());
        }
    }

    return sha;
}

} // namespace git
