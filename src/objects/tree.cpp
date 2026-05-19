#include "objects/tree.hpp"
#include "core/object_store.hpp"
#include "objects/blob.hpp"
#include "util/hex.hpp"

#include <algorithm>
#include <array>
#include <filesystem>

namespace objects {

auto Tree::create_tree_data(const std::vector<TreeEntry>& entries) -> std::string {
    std::string content;

    for (const auto& entry : entries) {
        content += entry.mode + " " + entry.name + '\0';
        auto sha_bytes = util::hex_to_bytes(entry.sha);
        if (!sha_bytes) {
            continue;
        }
        content += *sha_bytes;
    }

    return "tree " + std::to_string(content.size()) + '\0' + content;
}

auto Tree::write_tree(const std::filesystem::path& dir) -> std::expected<std::string, std::string> {
    std::vector<TreeEntry> entries;

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        std::string name = entry.path().filename().string();

        if (name == ".git") {
            continue;
        }

        if (entry.is_regular_file()) {
            auto sha = Blob::write_from_file(entry.path());
            if (!sha) {
                return std::unexpected(sha.error());
            }
            entries.push_back({"100644", name, *sha});
        } else if (entry.is_directory()) {
            auto sha = write_tree(entry.path());
            if (!sha) {
                return std::unexpected(sha.error());
            }
            entries.push_back({"40000", name, *sha});
        }
    }

    std::sort(entries.begin(), entries.end(), [](const TreeEntry& left, const TreeEntry& right) {
        return left.name < right.name;
    });

    std::string data = create_tree_data(entries);
    return core::ObjectStore::store_object(data);
}

auto Tree::parse(std::string_view raw_data) -> std::expected<std::vector<TreeEntry>, std::string> {
    size_t null_pos = raw_data.find('\0');
    if (null_pos == std::string_view::npos) {
        return std::unexpected("Invalid tree format: no header null byte");
    }

    std::string_view header = raw_data.substr(0, null_pos);
    if (!header.starts_with("tree ")) {
        return std::unexpected("Invalid tree format: expected 'tree' prefix");
    }

    std::string_view data = raw_data.substr(null_pos + 1);
    std::vector<TreeEntry> entries;

    while (!data.empty()) {
        size_t space_pos = data.find(' ');
        if (space_pos == std::string_view::npos) {
            return std::unexpected("Invalid tree entry: no mode found");
        }

        std::string mode(data.substr(0, space_pos));
        data = data.substr(space_pos + 1);

        size_t entry_null_pos = data.find('\0');
        if (entry_null_pos == std::string_view::npos) {
            return std::unexpected("Invalid tree entry: no name terminator");
        }

        std::string name(data.substr(0, entry_null_pos));
        data = data.substr(entry_null_pos + 1);

        if (data.size() < 20) {
            return std::unexpected("Invalid tree entry: insufficient SHA bytes");
        }

        std::string sha = util::bytes_to_hex(
            std::span<const std::uint8_t>(reinterpret_cast<const std::uint8_t*>(data.data()), 20));
        data = data.substr(20);

        entries.push_back({std::move(mode), std::move(name), std::move(sha)});
    }

    return entries;
}

} // namespace objects
