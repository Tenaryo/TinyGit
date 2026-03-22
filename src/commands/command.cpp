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
    return nullptr;
}

} // namespace git
