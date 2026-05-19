#pragma once

#include "commands/command.hpp"

namespace commands {

class LsTreeCommand : public Command {
  public:
    auto execute(std::span<std::string_view> args) -> int override;
};

} // namespace commands
