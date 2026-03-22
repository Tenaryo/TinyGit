#include "commands/cat_file.hpp"
#include "objects/blob.hpp"
#include "objects/object_store.hpp"

#include <iostream>

namespace git {

int CatFileCommand::execute(std::span<std::string_view> args) {
    if (args.size() < 2) {
        std::cerr << "Usage: cat-file -p <sha>\n";
        return 1;
    }

    std::string_view flag = args[0];
    std::string_view sha = args[1];

    if (flag != "-p") {
        std::cerr << "Unknown flag: " << flag << "\n";
        return 1;
    }

    auto raw_data = ObjectStore::read_object(sha);
    if (!raw_data) {
        std::cerr << raw_data.error() << "\n";
        return 1;
    }

    auto content = Blob::extract_content(*raw_data);
    if (!content) {
        std::cerr << content.error() << "\n";
        return 1;
    }

    std::cout.write(content->data(), static_cast<std::streamsize>(content->size()));
    return 0;
}

} // namespace git
