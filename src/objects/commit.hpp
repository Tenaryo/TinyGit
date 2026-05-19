#pragma once

#include <expected>
#include <string>
#include <string_view>

namespace objects::commit {

auto create_commit_data(std::string_view tree_sha,
                        std::string_view parent_sha,
                        std::string_view message) -> std::string;

auto write_commit(std::string_view tree_sha,
                  std::string_view parent_sha,
                  std::string_view message) -> std::expected<std::string, std::string>;

} // namespace objects::commit
