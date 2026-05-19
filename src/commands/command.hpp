#pragma once

#include <span>
#include <string_view>

namespace commands {

using Handler = int (*)(std::span<std::string_view>);

auto handle_init(std::span<std::string_view> args) -> int;
auto handle_cat_file(std::span<std::string_view> args) -> int;
auto handle_hash_object(std::span<std::string_view> args) -> int;
auto handle_ls_tree(std::span<std::string_view> args) -> int;
auto handle_write_tree(std::span<std::string_view> args) -> int;
auto handle_commit_tree(std::span<std::string_view> args) -> int;

auto run(std::string_view name, std::span<std::string_view> args) -> int;

} // namespace commands
