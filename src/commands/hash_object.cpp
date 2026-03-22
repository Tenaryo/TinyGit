#include "commands/hash_object.hpp"
#include "objects/blob.hpp"

#include <iostream>

namespace git {

int HashObjectCommand::execute(std::span<std::string_view> args) {
    bool write_flag = false;
    std::string_view file_path;

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-w") {
            write_flag = true;
        } else {
            file_path = args[i];
        }
    }

    if (file_path.empty()) {
        std::cerr << "Usage: hash-object [-w] <file>\n";
        return 1;
    }

    auto sha = Blob::write_from_file(std::string(file_path), write_flag);
    if (!sha) {
        std::cerr << sha.error() << "\n";
        return 1;
    }

    std::cout << *sha;
    return 0;
}

} // namespace git
