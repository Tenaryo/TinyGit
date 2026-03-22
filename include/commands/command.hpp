#pragma once

#include <memory>
#include <span>
#include <string_view>

namespace git {

class Command {
  public:
    virtual ~Command() = default;
    virtual int execute(std::span<std::string_view> args) = 0;

    static std::unique_ptr<Command> create(std::string_view name);
};

} // namespace git
