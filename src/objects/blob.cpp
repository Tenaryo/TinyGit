#include "objects/blob.hpp"

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

} // namespace git
