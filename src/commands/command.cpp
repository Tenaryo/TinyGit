#include "git.hpp"

#include <iostream>

namespace git {

std::unique_ptr<Command> Command::create(std::string_view name) {
    if (name == "cat-file") {
        return std::make_unique<CatFileCommand>();
    }
    if (name == "hash-object") {
        return std::make_unique<HashObjectCommand>();
    }
    if (name == "init") {
        return std::make_unique<InitCommand>();
    }
    if (name == "ls-tree") {
        return std::make_unique<LsTreeCommand>();
    }
    if (name == "write-tree") {
        return std::make_unique<WriteTreeCommand>();
    }
    return nullptr;
}

} // namespace git
