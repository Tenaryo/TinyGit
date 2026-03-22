#include "objects/tree.hpp"
#include "objects/blob.hpp"
#include "objects/object_store.hpp"

#include <algorithm>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace git {

std::string hex_to_bytes(std::string_view hex) {
    std::string bytes;
    for (size_t i = 0; i < hex.size(); i += 2) {
        unsigned int byte = 0;
        std::stringstream ss;
        ss << std::hex << hex.substr(i, 2);
        ss >> byte;
        bytes += static_cast<char>(byte);
    }
    return bytes;
}

std::string Tree::create_tree_data(const std::vector<TreeEntry>& entries) {
    std::string content;

    for (const auto& entry : entries) {
        content += entry.mode + " " + entry.name + '\0';
        content += hex_to_bytes(entry.sha);
    }

    return "tree " + std::to_string(content.size()) + '\0' + content;
}

std::expected<std::string, std::string> Tree::write_tree(const std::filesystem::path& dir) {
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

    std::sort(entries.begin(), entries.end(), [](const TreeEntry& a, const TreeEntry& b) {
        return a.name < b.name;
    });

    std::string data = create_tree_data(entries);
    std::string sha = ObjectStore::compute_sha1(data);
    std::string compressed = ObjectStore::compress(data);
    auto result = ObjectStore::write_object(sha, compressed);
    if (!result) {
        return std::unexpected(result.error());
    }

    return sha;
}

std::expected<std::vector<TreeEntry>, std::string> Tree::parse(std::string_view raw_data) {
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

        std::ostringstream oss;
        for (size_t i = 0; i < 20; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(static_cast<unsigned char>(data[i]));
        }
        std::string sha = oss.str();
        data = data.substr(20);

        entries.push_back({std::move(mode), std::move(name), std::move(sha)});
    }

    std::sort(entries.begin(), entries.end(), [](const TreeEntry& a, const TreeEntry& b) {
        return a.name < b.name;
    });

    return entries;
}

} // namespace git
