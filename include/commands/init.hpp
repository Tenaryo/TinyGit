#pragma once

#include "command.hpp"

namespace git {

class InitCommand : public Command {
  public:
    int execute(std::span<std::string_view> args) override;
};

} // namespace git
