#include "commands/command.hpp"

#include <unordered_map>

namespace commands {

auto run(std::string_view name, std::span<std::string_view> args) -> int {
    static const std::unordered_map<std::string_view, Handler> kRegistry = {
        {"cat-file", handle_cat_file},
        {"commit-tree", handle_commit_tree},
        {"hash-object", handle_hash_object},
        {"init", handle_init},
        {"ls-tree", handle_ls_tree},
        {"write-tree", handle_write_tree},
    };

    auto it = kRegistry.find(name);
    if (it == kRegistry.end()) {
        return -1;
    }
    return it->second(args);
}

} // namespace commands
