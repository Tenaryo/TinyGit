#include "objects/tree.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace git {

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
