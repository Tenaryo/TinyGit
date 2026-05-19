#include "objects/commit.hpp"
#include "core/object_store.hpp"

namespace objects::commit {

auto create_commit_data(std::string_view tree_sha,
                        std::string_view parent_sha,
                        std::string_view message) -> std::string {
    std::string content;
    content += "tree " + std::string(tree_sha) + "\n";
    content += "parent " + std::string(parent_sha) + "\n";
    content += "author User <user@example.com> 1234567890 +0000\n";
    content += "committer User <user@example.com> 1234567890 +0000\n";
    content += "\n";
    content += std::string(message) + "\n";

    return "commit " + std::to_string(content.size()) + '\0' + content;
}

auto write_commit(std::string_view tree_sha,
                  std::string_view parent_sha,
                  std::string_view message) -> std::expected<std::string, std::string> {
    std::string data = create_commit_data(tree_sha, parent_sha, message);
    return core::store_object(data);
}

} // namespace objects::commit
