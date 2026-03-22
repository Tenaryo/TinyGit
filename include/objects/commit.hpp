#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace git {

class Commit {
  public:
    static std::string create_commit_data(std::string_view tree_sha,
                                          std::string_view parent_sha,
                                          std::string_view message);

    static std::expected<std::string, std::string>
    write_commit(std::string_view tree_sha, std::string_view parent_sha, std::string_view message);
};

} // namespace git
