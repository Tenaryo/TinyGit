#include "commands/commit_tree.hpp"
#include "objects/commit.hpp"

#include <iostream>

namespace git {

int CommitTreeCommand::execute(std::span<std::string_view> args) {
    std::string_view tree_sha;
    std::string_view parent_sha;
    std::string_view message;

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-p" && i + 1 < args.size()) {
            parent_sha = args[++i];
        } else if (args[i] == "-m" && i + 1 < args.size()) {
            message = args[++i];
        } else if (tree_sha.empty()) {
            tree_sha = args[i];
        }
    }

    if (tree_sha.empty() || parent_sha.empty() || message.empty()) {
        std::cerr << "Usage: commit-tree <tree-sha> -p <parent-sha> -m <message>\n";
        return 1;
    }

    auto sha = Commit::write_commit(tree_sha, parent_sha, message);
    if (!sha) {
        std::cerr << sha.error() << "\n";
        return 1;
    }

    std::cout << *sha;
    return 0;
}

} // namespace git
