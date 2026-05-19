#include "commands/ls_tree.hpp"
#include "objects/object_store.hpp"
#include "objects/tree.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace git {

namespace {
auto format_mode(const std::string& mode) -> std::string {
    std::ostringstream oss;
    oss << std::setw(6) << std::setfill('0') << mode;
    return oss.str();
}
} // namespace

auto LsTreeCommand::execute(std::span<std::string_view> args) -> int {
    bool name_only = false;
    std::string_view sha;

    for (auto arg : args) {
        if (arg == "--name-only") {
            name_only = true;
        } else {
            sha = arg;
        }
    }

    if (sha.empty()) {
        std::cerr << "Usage: ls-tree [--name-only] <tree-sha>\n";
        return 1;
    }

    auto raw_data = ObjectStore::read_object(sha);
    if (!raw_data) {
        std::cerr << raw_data.error() << "\n";
        return 1;
    }

    auto entries = Tree::parse(*raw_data);
    if (!entries) {
        std::cerr << entries.error() << "\n";
        return 1;
    }

    for (const auto& entry : *entries) {
        if (name_only) {
            std::cout << entry.name << "\n";
        } else {
            std::cout << format_mode(entry.mode) << " " << entry.type_name() << " " << entry.sha
                      << "\t" << entry.name << "\n";
        }
    }

    return 0;
}

} // namespace git
