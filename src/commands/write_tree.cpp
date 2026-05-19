#include "commands/write_tree.hpp"
#include "objects/tree.hpp"

#include <filesystem>
#include <iostream>

namespace commands {

auto WriteTreeCommand::execute([[maybe_unused]] std::span<std::string_view> args) -> int {
    auto sha = objects::Tree::write_tree(std::filesystem::current_path());
    if (!sha) {
        std::cerr << sha.error() << "\n";
        return 1;
    }

    std::cout << *sha;
    return 0;
}

} // namespace commands
