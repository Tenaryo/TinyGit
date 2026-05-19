#include "commands/command.hpp"

#include <iostream>
#include <vector>

auto main(int argc, char* argv[]) -> int {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 2) {
        std::cerr << "No command provided.\n";
        return EXIT_FAILURE;
    }

    std::vector<std::string_view> args;
    for (int i = 2; i < argc; ++i) {
        args.emplace_back(argv[i]);
    }

    int result = commands::run(argv[1], args);
    if (result < 0) {
        std::cerr << "Unknown command " << argv[1] << '\n';
        return EXIT_FAILURE;
    }
    return result;
}
