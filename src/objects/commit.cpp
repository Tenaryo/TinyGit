#include "objects/commit.hpp"
#include "objects/object_store.hpp"

namespace git {

std::string Commit::create_commit_data(std::string_view tree_sha,
                                       std::string_view parent_sha,
                                       std::string_view message) {
    std::string content;
    content += "tree " + std::string(tree_sha) + "\n";
    content += "parent " + std::string(parent_sha) + "\n";
    content += "author User <user@example.com> 1234567890 +0000\n";
    content += "committer User <user@example.com> 1234567890 +0000\n";
    content += "\n";
    content += std::string(message) + "\n";

    return "commit " + std::to_string(content.size()) + '\0' + content;
}

std::expected<std::string, std::string> Commit::write_commit(std::string_view tree_sha,
                                                             std::string_view parent_sha,
                                                             std::string_view message) {
    std::string data = create_commit_data(tree_sha, parent_sha, message);
    std::string sha = ObjectStore::compute_sha1(data);
    std::string compressed = ObjectStore::compress(data);
    auto result = ObjectStore::write_object(sha, compressed);
    if (!result) {
        return std::unexpected(result.error());
    }

    return sha;
}

} // namespace git
