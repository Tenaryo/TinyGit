#include "commands/command.hpp"
#include "objects/blob.hpp"

#include <iostream>

namespace commands {

auto handle_hash_object(std::span<std::string_view> args) -> int {
    bool write_flag = false;
    std::string_view file_path;

    for (auto arg : args) {
        if (arg == "-w") {
            write_flag = true;
        } else {
            file_path = arg;
        }
    }

    if (file_path.empty()) {
        std::cerr << "Usage: hash-object [-w] <file>\n";
        return 1;
    }

    auto sha = objects::blob::write_from_file(std::string(file_path), write_flag);
    if (!sha) {
        std::cerr << sha.error() << "\n";
        return 1;
    }

    std::cout << *sha;
    return 0;
}

} // namespace commands
