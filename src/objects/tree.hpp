#pragma once

#include <cstdint>
#include <expected>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace objects::tree {

enum class ObjectType : std::uint8_t { Tree, Blob };

struct TreeEntry {
    std::string mode;
    std::string name;
    std::string sha;

    [[nodiscard]] auto type() const -> ObjectType {
        return mode == "40000" ? ObjectType::Tree : ObjectType::Blob;
    }

    [[nodiscard]] auto type_name() const -> std::string_view {
        return mode == "40000" ? "tree" : "blob";
    }
};

auto parse(std::string_view raw_data) -> std::expected<std::vector<TreeEntry>, std::string>;
auto create_tree_data(const std::vector<TreeEntry>& entries) -> std::string;
auto write_tree(const std::filesystem::path& dir) -> std::expected<std::string, std::string>;

} // namespace objects::tree
