#include "commands/init.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace git {

int InitCommand::execute([[maybe_unused]] std::span<std::string_view> args) {
    try {
        std::filesystem::create_directory(".git");
        std::filesystem::create_directory(".git/objects");
        std::filesystem::create_directory(".git/refs");

        std::ofstream head_file(".git/HEAD");
        if (head_file.is_open()) {
            head_file << "ref: refs/heads/main\n";
            head_file.close();
        } else {
            std::cerr << "Failed to create .git/HEAD file.\n";
            return 1;
        }

        std::cout << "Initialized git directory\n";
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

    return 0;
}

} // namespace git
