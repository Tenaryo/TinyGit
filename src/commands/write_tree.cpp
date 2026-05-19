#include "commands/command.hpp"
#include "objects/tree.hpp"

#include <filesystem>
#include <iostream>

namespace commands {

auto handle_write_tree([[maybe_unused]] std::span<std::string_view> args) -> int {
    auto sha = objects::tree::write_tree(std::filesystem::current_path());
    if (!sha) {
        std::cerr << sha.error() << "\n";
        return 1;
    }

    std::cout << *sha;
    return 0;
}

} // namespace commands
