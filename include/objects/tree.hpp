#pragma once

#include <expected>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace git {

enum class ObjectType { Tree, Blob };

struct TreeEntry {
    std::string mode;
    std::string name;
    std::string sha;

    ObjectType type() const { return mode == "40000" ? ObjectType::Tree : ObjectType::Blob; }

    std::string_view type_name() const { return mode == "40000" ? "tree" : "blob"; }
};

class Tree {
  public:
    static std::expected<std::vector<TreeEntry>, std::string> parse(std::string_view raw_data);

    static std::string create_tree_data(const std::vector<TreeEntry>& entries);

    static std::expected<std::string, std::string> write_tree(const std::filesystem::path& dir);
};

std::string hex_to_bytes(std::string_view hex);

} // namespace git
