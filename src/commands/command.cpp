#include "commands/command.hpp"
#include "commands/cat_file.hpp"
#include "commands/commit_tree.hpp"
#include "commands/hash_object.hpp"
#include "commands/init.hpp"
#include "commands/ls_tree.hpp"
#include "commands/write_tree.hpp"

#include <unordered_map>

namespace commands {

auto Command::create(std::string_view name) -> std::unique_ptr<Command> {
    using Factory = std::unique_ptr<Command> (*)();
    static const std::unordered_map<std::string_view, Factory> kRegistry = {
        {"cat-file",
         []() -> std::unique_ptr<Command> { return std::make_unique<CatFileCommand>(); }},
        {"commit-tree",
         []() -> std::unique_ptr<Command> { return std::make_unique<CommitTreeCommand>(); }},
        {"hash-object",
         []() -> std::unique_ptr<Command> { return std::make_unique<HashObjectCommand>(); }},
        {"init", []() -> std::unique_ptr<Command> { return std::make_unique<InitCommand>(); }},
        {"ls-tree", []() -> std::unique_ptr<Command> { return std::make_unique<LsTreeCommand>(); }},
        {"write-tree",
         []() -> std::unique_ptr<Command> { return std::make_unique<WriteTreeCommand>(); }},
    };

    auto it = kRegistry.find(name);
    return it != kRegistry.end() ? it->second() : nullptr;
}

} // namespace commands
