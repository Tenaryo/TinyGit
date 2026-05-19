#pragma once

#include <memory>
#include <span>
#include <string_view>

namespace git {

class Command {
  public:
    virtual ~Command() = default;
    virtual auto execute(std::span<std::string_view> args) -> int = 0;

    static auto create(std::string_view name) -> std::unique_ptr<Command>;
};

} // namespace git
