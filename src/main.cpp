#include "git.hpp"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 2) {
        std::cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }

    auto cmd = git::Command::create(argv[1]);
    if (!cmd) {
        std::cerr << "Unknown command " << argv[1] << '\n';
        return EXIT_FAILURE;
    }

    std::vector<std::string_view> args;
    for (int i = 2; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    return cmd->execute(args);
}
