#pragma once

#include "SendableCommandBase.h"
#include "CommandHelper.h"

namespace frc {
namespace experimental {
class InstantCommand : public CommandHelper<SendableCommandBase, InstantCommand> {
 public:
  InstantCommand(std::function<void()> toRun, wpi::ArrayRef<Subsystem*> requirements) : m_toRun{std::move(toRun)} {
    AddRequirements(requirements);
  }

  InstantCommand(InstantCommand&& other) = default;

  InstantCommand() : m_toRun{[]{}} {
  }

  void b() {

  }

  void Initialize() override {
    m_toRun();
  }

  bool IsFinished() final override {
    return true;
  }

 private:
  std::function<void()> m_toRun;
};
}
}
