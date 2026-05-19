#pragma once

#include "command.hpp"

namespace git {

class CommitTreeCommand : public Command {
  public:
    auto execute(std::span<std::string_view> args) -> int override;
};

} // namespace git
