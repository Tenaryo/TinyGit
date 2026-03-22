#include "commands/hash_object.hpp"
#include "objects/blob.hpp"
#include "objects/object_store.hpp"

#include <filesystem>
#include <fstream>
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

    std::ifstream file(std::string(file_path), std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << file_path << "\n";
        return 1;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    auto blob_data = Blob::create_blob_data(content);
    if (!blob_data) {
        std::cerr << blob_data.error() << "\n";
        return 1;
    }

    std::string sha = ObjectStore::compute_sha1(*blob_data);

    if (write_flag) {
        std::string compressed = ObjectStore::compress(*blob_data);
        auto result = ObjectStore::write_object(sha, compressed);
        if (!result) {
            std::cerr << result.error() << "\n";
            return 1;
        }
    }

    std::cout << sha;
    return 0;
}

} // namespace git
